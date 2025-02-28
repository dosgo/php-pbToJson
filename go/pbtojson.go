package main

/*
#include <stdlib.h>
#include <string.h>
*/
import "C"
import (
	"context"
	"encoding/json"
	"unsafe"

	"github.com/bufbuild/protocompile"
	"github.com/bufbuild/protocompile/linker"
	"google.golang.org/protobuf/encoding/protojson"
	"google.golang.org/protobuf/proto"
	gproto "google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/reflect/protoreflect"
	"google.golang.org/protobuf/types/dynamicpb"
	"google.golang.org/protobuf/types/known/anypb"
)

var pbFiles linker.Files

//export pbCompile
func pbCompile(jsonConf *C.char) C.int {
	jsonData := C.GoString(jsonConf) //

	var result map[string]string
	// 解析 JSON 数据到 map 中
	err := json.Unmarshal([]byte(jsonData), &result)
	if err != nil {
		return -1
	}

	var pbFileNames []string
	//检查为空
	for key, value := range result {
		if key == "" || value == "" {
			delete(result, key)
		}
		pbFileNames = append(pbFileNames, key)
	}
	if len(result) < 1 {
		return -2
	}

	compiler := &protocompile.Compiler{
		Resolver: protocompile.WithStandardImports(&protocompile.SourceResolver{
			Accessor: protocompile.SourceAccessorFromMap(result),
		}),
	}
	pbFiles, err = compiler.Compile(context.Background(), pbFileNames...)
	if err != nil {
		return -3
	}
	return 0
}

//export anypbToJson
func anypbToJson(data *C.char, dataLen uint32) *C.char {
	goData := C.GoBytes(unsafe.Pointer(data), C.int(dataLen)) //
	var anyMsg anypb.Any
	// 尝试解析二进制数据到 Any 消息中
	if err := gproto.Unmarshal(goData, &anyMsg); err != nil {
		return C.CString("")
	}
	backMsg := make(map[string]interface{})
	backMsg["Value"] = anyMsg.GetValue()
	backMsg["TypeUrl"] = anyMsg.GetTypeUrl()
	jsonBytes, err := json.Marshal(backMsg)
	if err != nil {
		return C.CString("")
	}
	return C.CString(string(jsonBytes))
}

//export anypbToJsonBuf
func anypbToJsonBuf(data *C.char, dataLen uint32, out *C.char, outLen uint32) C.int {
	if dataLen <= 0 || outLen <= 0 {
		return C.int(-1)
	}
	var res *C.char = anypbToJson(data, dataLen)

	if int(C.strlen(res)) > int(outLen) {
		return C.int(-2)
	}
	C.memcpy(
		unsafe.Pointer(out),       // 目标指针
		unsafe.Pointer(res),       // 源指针
		C.size_t(C.strlen(res)+1), // 复制长度（含 \0）
	)
	// 返回 JSON 字符串
	return C.int(C.strlen(res))
}

//export pbToJson
func pbToJson(data *C.char, dataLen uint32, pbFile *C.char, typeUrl *C.char) *C.char {
	goData := C.GoBytes(unsafe.Pointer(data), C.int(dataLen)) //
	// 动态解析 .proto 文件
	if pbFiles == nil {
		return C.CString("")
	}

	fileDesc := pbFiles.FindFileByPath(C.GoString(pbFile))
	if fileDesc == nil {
		return C.CString("")
	}
	// 4. 获取 "Person" 消息类型的描述符
	personDesc := fileDesc.Messages().ByName(protoreflect.Name(C.GoString(typeUrl)))
	if personDesc == nil {
		return C.CString("")
	}
	// 5. 创建一个动态消息实例
	personMsg := dynamicpb.NewMessage(personDesc)

	// 7. 反序列化二进制数据到动态消息
	if err := proto.Unmarshal(goData, personMsg); err != nil {
		return C.CString("")
	}
	jsStr, err := protojson.Marshal(personMsg)
	//把test.AddFriendReq 消息体序列化成 JSON 数据
	if err != nil {
		return C.CString("")
	}
	return C.CString(string(jsStr))
}

//export pbToJsonBuf
func pbToJsonBuf(data *C.char, dataLen uint32, pbFile *C.char, typeUrl *C.char, out *C.char, outLen uint32) C.int {
	if dataLen <= 0 || outLen <= 0 {
		return C.int(-1)
	}
	var res *C.char = pbToJson(data, dataLen, pbFile, typeUrl)

	if int(C.strlen(res)) > int(outLen) {
		return C.int(-2)
	}
	C.memcpy(
		unsafe.Pointer(out),       // 目标指针
		unsafe.Pointer(res),       // 源指针
		C.size_t(C.strlen(res)+1), // 复制长度（含 \0）
	)
	// 返回 JSON 字符串
	return C.int(C.strlen(res))
}

func main() {}
