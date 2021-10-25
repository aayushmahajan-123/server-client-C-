#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstring>
#include <sstream>
#include<fstream>
#include <regex>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<chrono>
using namespace std;


class data{
    public:
    int opt;
    int n;
    string arr;
    data(string opt,string n,string arr){
        this->opt = stoi(opt);
        this->n = stoi(n);
        regex r("\\s+");
        this->arr= regex_replace(arr, r, "");
    }
};

string sort_arr(int arr[],int n){
    sort(arr,arr+n);
    string temp = "";
    for(int i=0;i<n;i++){
        temp+=to_string(arr[i])+" ";
    }
    return temp;
}

string find_max_occurance(int arr[],int n){
    unordered_map<int, int> hash;
    for (int i = 0; i < n; i++)
        hash[arr[i]]++;
 
    int max_count = 0, res = -1;
    for (auto i : hash) {
        if(max_count==i.second and res>i.first){
            res=i.first;
        }
        else if (max_count < i.second) {
            res = i.first;
            max_count = i.second;
        }
    }
    return to_string(res);
}

string find_max(int arr[],int n){
    int ans = arr[0];
    for(int i=0;i<n;i++){
        ans = max(ans,arr[i]);
    }
    return to_string(ans);
}

string find_min(int arr[],int n){
    int ans = arr[0];
    for(int i=0;i<n;i++){
        ans = min(ans,arr[i]);
    }
    return to_string(ans);
}

string find_mean(int arr[],int n){
    int s = 0;
    for(int i=0;i<n;i++){
        s+=arr[i];
    }
    float mean = (float)(s)/n;
    float value = (int)(mean * 100 + .5);
    return to_string((float)value / 100);
}

string find_answer(data* Data){
    cout<<Data->opt<<" "<<Data->n<<endl;
  
    stringstream string_stream(Data->arr);           
    int arr[10000];
    int i=0;
	while(string_stream.good())   
	{
		string a;       
		getline( string_stream, a, ',');  
		arr[i]=stoi(a);
        i++;
	}
    
    string ans ;
    switch(Data->opt){
        case 1:
            ans = sort_arr(arr,Data->n);
        break;
        case 2:
            ans = find_max_occurance(arr,Data->n);
        break;
        case 3:
            ans = find_max(arr,Data->n);
        break;
        case 4:
            ans = find_min(arr,Data->n);
        break;
        case 5:
            ans = find_mean(arr,Data->n);
        break;
        default :
        exit(0);
    }
    cout<<ans<<endl;
    
    return ans;
}


string handle_message(string &msg){
 
    istringstream f(msg);
    string line="";
    getline(f, line);
    int problems = stoi(line);
    string answer = "";
    for(int i=0;i<problems;i++){
        getline(f, line);
        string optn = line;
        getline(f, line);
        string N = line;
        getline(f, line);
        string arr = line;

        data Data = data(optn,N,arr);
        data* temp = &Data;
        answer+=find_answer(temp)+"\n";
    }
    return answer;

}




void handle_connection(int client_socket){
    char buf[4096];

    memset(buf,4096,0);
    //read the message
    int bytesrec = recv(client_socket,buf,4096,0);

    string response;
    if(bytesrec>0){
        
        string msg(buf);

        response = handle_message(msg);
        //sleep(20);
        int bytes_sent = send(client_socket , response.c_str() , response.length() , 0 );
    }
    close(client_socket);
}




int main(int argc, char const *argv[]){

    cout<<argc<<endl;
    if(argc!=3){
        cerr<<"server cannot run without ip and port";
        return 0;
    }
    else{
        cout<<argv[1]<<endl;
        cout<<argv[2]<<endl;
    }



    int listening_socket , client_socket;
    listening_socket = socket(AF_INET,SOCK_STREAM,0);
    if(listening_socket==-1){
        cerr<<"error in socket";
        return 0;
    }

    sockaddr_in server,client;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = inet_addr(argv[1]);
    memset(server.sin_zero,8,0);

    unsigned len = sizeof(sockaddr_in);

    if((bind(listening_socket,(sockaddr*)&server,len))==-1){
        cerr<<"error bind";
        return 0;
    }


    if(listen(listening_socket,5)==-1){
        cerr<<"error listen";
        return 0;
    }

    while(1){
        cout<<"\n\nSend the query!!"<<endl;

        if((client_socket = accept(listening_socket,(sockaddr*)&client,&len))==-1){
            cerr<<"error accept";
            return 0;
        }
        cout<<"connected"<<endl;

        handle_connection(client_socket);   

    }

    return 0;
}







