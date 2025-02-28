<?php

$pbContents=file_get_contents('../testdata/desc_test1.proto');

$res=pbToJson::pbCompile(json_encode(['test.proto'=>$pbContents]));
if($res!=0){
    echo "Compile Error\r\n";
    return ;
}

     
//bin data
$binData1="";
$msgName="";
//pbToJson::toJson(bindate,protoFilename,MessageName);
$jsonstr= pbToJson::toJson($binData1,"test.proto","TestMessage");
 



//bin data
$binData="";

$pbdata=json_decode(pbToJson::anyToJson($binData),true);
// var_dump($pbdata);
$ddd=base64_decode($pbdata['Value']);
var_dump($pbdata['TypeUrl']);
