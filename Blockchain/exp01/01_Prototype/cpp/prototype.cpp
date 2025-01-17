#include <iostream>
#include <ctime>	// http://www.cplusplus.com/reference/ctime/time/
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#define u64 unsigned long long
using namespace std;

// https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
// https://stackoverflow.com/questions/13784434/how-to-use-openssls-sha256-functions
// https://www.xorbin.com/tools/sha256-hash-calculator
string getsha256(string str){
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);
	stringstream ss;
	for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
		ss << hex << setw(2) << setfill('0') << (int)hash[i];
	}
	return ss.str();
}


// 将字节数组，转换成字符串
// 举例
// 41 -> 0x41 -> 65 -> A
// 字节数组用字符串表示，是为了方便打印查看 hash 
string byte2str(string strByteArray){
	stringstream ss;
	for( int i = 0; i < strByteArray.size(); i += 2 ){
		string strhex = strByteArray.substr(i, 2);
		// https://stackoverflow.com/questions/17261798/converting-a-hex-string-to-a-byte-array
		// http://www.cplusplus.com/reference/cstdlib/strtoul/?kw=strtoul
		ss<<(unsigned char)strtoul(strhex.c_str(), NULL, 16);
	}
	string s = ss.str();
	return s;
}

struct Block{
	string timeStamp;	// 时间戳, 记录区块生成的时间
	string prevHash;	// 前一个区块的 hash, 是一个以字符串形式保存的字节数组 byte array
	string currhash;	// 当前区块的 hash, 是一个以字符串形式保存的字节数组 byte array
	string msg;		// 区块中存储的实际交易
	Block * next;

	Block(string _msg, string _prevHash){
		timeStamp = to_string(0);
		// time_t timer;
		// time(&timer);
		// timeStamp = to_string( (u64)timer );
		
		prevHash = _prevHash;
		msg = _msg;
		getHash();
		next = NULL;
	}

	void getHash(){
		string data = byte2str(prevHash) + msg + timeStamp;
		currhash = getsha256(data);
	}
};

struct BlockChain{
	Block *head, *tail;
	size_t size;

	BlockChain(){
		head = new Block("Genesis Block", "");
		tail = head;
		size = 1;
	}

	int addBlock(string msg){
		cout<<"prev hash:\t"<<tail->currhash<<'\n';
		cout<<"curr msg:\t"<<msg<<'\n';
		tail->next = new Block(msg, tail->currhash);
		tail = tail->next;
		size++;
	}
};

int main(int argc, char *argv[]){
	BlockChain * bc = new BlockChain();
	bc->addBlock("send 1 BTC to Ivan");
	bc->addBlock("send 2 more BTC to Ivan");

	cout<<"\n\n\n";
	Block * p = bc->head;
	for( int i = 0; i < bc->size; i++ ){
		cout<<"prev hash: "<<p->prevHash<<'\n';
		cout<<"curr msg: "<<p->msg<<'\n';
		cout<<"curr Hash: "<<p->currhash<<"\n\n\n";
		p = p->next;
	}

	return 0;
}

/*
string toASCIIByte(u64 num){
	string s = to_string(num);
	stringstream ss;
	for( char ch : s ){
		ss<<hex<<setw(2)<<setfill('0')<<(int)ch;
	}
	return ss.str();
}
*/
/*
string toASCIIByte(string s){
	stringstream ss;
	for( char ch : s ){
		ss<<hex<<setw(2)<<setfill('0')<<(int)ch;
	}
	return ss.str();
}
*/

// TODO
// 1. 设置固定时间，对比 go 和 cpp 的结果
// 2. 增加 pow
// 3. 数据库(或者自己写保存文件)
// 4. 命令行
// 5. Python
