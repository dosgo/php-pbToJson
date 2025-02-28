<?php


// 加载共享库
$ffi = FFI::cdef("
    int pbCompile(const char* jsonConf);
    char* anypbToJson(const char* data,  uint32_t dataLen);
    char* pbToJson(const char* data,  uint32_t dataLen, const char* pbFile,const char* typeUrl);
", "./gopbtojson.so");



$pbContents=file_get_contents('../testdata/desc_test1.proto');

$res=$ffi->pbCompile(json_encode(['test.proto'=>$pbContents]));
if($res!=0){
    echo "Compile Error\r\n";
    return ;
}

     
//bin data
$binData1="11";
$msgName="";
//pbToJson::toJson(bindate,protoFilename,MessageName);
$jsonstr= $ffi->pbToJson($binData1,strlen($binData1), "test.proto","TestMessage");
 




//bin data
$binData="";

$pbdata=json_decode($ffi->anypbToJson($binData,strlen($binData)),true);
// var_dump($pbdata);
$ddd=base64_decode($pbdata['Value']);
var_dump($pbdata['TypeUrl']);
