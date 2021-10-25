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
#include<queue>
#include<pthread.h>
using namespace std;

/* A datastructure made to manage the incoming data from the client
    It stores the type of problem, number of terms in the array , and the array 
    of a particular problem*/
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

//sort the array
string sort_arr(int arr[],int n){
    sort(arr,arr+n);
    string temp = "";
    for(int i=0;i<n;i++){
        temp+=to_string(arr[i])+" ";
    }
    return temp;
}

// find the max occurance in the array
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

// Find the max value
string find_max(int arr[],int n){
    int ans = arr[0];
    for(int i=0;i<n;i++){
        ans = max(ans,arr[i]);
    }
    return to_string(ans);
}

//find the min value
string find_min(int arr[],int n){
    int ans = arr[0];
    for(int i=0;i<n;i++){
        ans = min(ans,arr[i]);
    }
    return to_string(ans);
}

//find the mean of the array
string find_mean(int arr[],int n){
    int s = 0;
    for(int i=0;i<n;i++){
        s+=arr[i];
    }
    float mean = (float)(s)/n;
    float value = (int)(mean * 100 + .5);
    return to_string((float)value / 100);
}



/* A function which takes 1 question at a time and returns the answer for it
    In the form of a string*/
string find_answer(data* Data){
    //cout<<Data->opt<<" "<<Data->n<<endl;
    

    //Convert the string into an array of integers
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
    

    //Call the relevant function for the problem
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


/* Take a particular problem and store its values in the Data Class created by us
    After storing the data send the data to the find_answer() function and return the answer*/
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



/*Take in the Socket Pointer and receive and send the data, fill them in the buffer*/
void* handle_connection(void* client_socket_pointer){
    int client_socket = *(int*)client_socket_pointer;
    //delete(client_socket_pointer);

    char buf[4096];
    

    memset(buf,4096,0);
    //read the message
    int bytesrec = recv(client_socket,buf,4096,0);

    string response;
    if(bytesrec>0){
        
        string msg(buf);

        response = handle_message(msg);
       // sleep(20);
        int bytes_sent = send(client_socket , response.c_str() , response.length() , 0 );
    }
    close(client_socket);

    return NULL;
}


int main(int argc, char const *argv[]){


    //Chgeck for required arguments and values
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

        if((client_socket = accept(listening_socket,(sockaddr*)&client,&len))==-1){
            cerr<<"error accept";
            return 0;
        }
    /*Create thread for an incoming problem and solve the answer via multithreading*/
        pthread_t t;
        int *pclient = new int;
        *pclient = client_socket;
        pthread_create(&t,NULL,handle_connection,pclient);

    }

    return 0;
}







