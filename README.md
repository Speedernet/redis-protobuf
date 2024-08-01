Installation:
```bash
git clone https://github.com/ygrandgirard/redis-protobuf.git

cd redis-protobuf/protobuf-21.12

# Install a slightly modified version of the Protobuf library
./configure "CFLAGS=-fPIC" "CXXFLAGS=-fPIC"
make -j 4
sudo make install

cd ..

# Install a slightly modified version of Redis-Protobuf module
mkdir compile
cd compile
cmake ..
make
sudo cp libredis-protobuf.so /usr/local/lib/

# Edit the Redis configuration file and restart the server
echo "loadmodule /usr/local/lib/libredis-protobuf.so --dir /var/lib/redis/protos" | sudo tee -a /etc/redis/redis.conf
sudo install -d -o redis -g redis /var/lib/redis/protos
sudo service redis-server restart
```

For the full documentation, please refer to the [original repository](https://github.com/sewenew/redis-protobuf/tree/master).

