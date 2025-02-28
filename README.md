# php-pbtojson


##ffi build

cd ffi

go build -buildmode=c-shared -o gopbtojson.so  ..\go\pbtojson.go


##ext build

cd ext

go build -buildmode=c-archive -o libgopbtojson.a  ..\go\pbtojson.go

phpize

./configure 

make

sudo make install




