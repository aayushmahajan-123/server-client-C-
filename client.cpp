#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstring>
#include<fstream>

using namespace std;




void transfer_file(const char* filename, int client){
    
    fstream file;
    file.open(filename, ios::in | ios::binary);
    if(file.is_open()){
         cout<<"opened file"<<endl;
    }
    else{
        cerr<<"failed to open file";
        exit(0);
    }

    string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    int bytes_sent = send(client , contents.c_str() , contents.length() , 0 );
    if(bytes_sent==-1){
        cerr<<"error sending";
        exit(1);
    }
    
}


int main(int argc, char const *argv[]){

    if(argc != 5){
        cerr<<"not enough arguments";
        return 0;
    }
    string op_file = argv[4];
    const char *ip_file = argv[3],*ipAddress = argv[1];
    int port = atoi(argv[2]);

    int client = socket(AF_INET,SOCK_STREAM,0);
    if(client==-1){
        cerr<<"error socket";
        return 0;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    hint.sin_addr.s_addr = inet_addr(ipAddress);


    int conresult = connect(client,(sockaddr*)&hint,sizeof(hint));
    if(conresult==-1){
        cerr<<"error connect";
        return 0;
    }
    
    char buf[4096];
    

    transfer_file(ip_file,client);


    int bytesrec = recv(client,buf,4096,0);
    if(bytesrec>0){
        string msg(buf);
        msg+="\n\n";
        ofstream outfile;
        outfile.open(op_file,ios_base::app);
        outfile<<msg;
        cout<<msg<<endl;
    }

    close(client);
    

    return 0;
}