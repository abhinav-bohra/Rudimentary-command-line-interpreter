/*------------------------------------------------------------------------------------------------------------------------------

	Operating Systems Laboratory (CS39002) - Spring	Semester 2020-2021

	Assignment 2: Implementation of a rudimentary command-line interpreter running on the Linux operating system

	Group Number  : 31
	Animesh Jain  | 18CS10004
	Abhinav Bohra | 18CS30049

-------------------------------------------------------------------------------------------------------------------------------*/

//Including Header Files
#include <bits/stdc++.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------
//Function Prototyping
string trim(string str);
vector<string> splitCommand(string cmd, char delim);
vector<string> readCommand(bool &background);
vector<string> split_Redirection(string cmd);
void Redirect_IO(string inputFile, string outputFile);
void executeCommand(string cmd);

//---------------------------------------------------------------------------------------------------------------------------------
//Function Definitions

// Function to trim all extra whitespaces of the string 'str'
string trim(string str){

	for(int i=str.size()-1; i >= 0; i-- ){

    		if(str[i]==' ' && str[i]==str[i-1])  str.erase( str.begin() + i );
	}

	//Trim leading & trailing whitespace
	if(str[0]==' ') str.erase( str.begin());
	if(str[str.size()-1]==' ') str.erase( str.begin() + str.size()-1);

	return str;
}

// Function to split string by a delimeter
vector<string> splitCommand(string cmd, char delim){


    vector<string> commands;
    // Create a string stream from the command
    stringstream ss(cmd); 
    string str;

    // Read from the string stream till the delimiter
    while(getline(ss, str, delim))
        commands.push_back(str);
    
    return commands;
}
 

// Function to read and preprocess commmand
vector<string> readCommand(bool &background){
	
	string cmd;
	vector<string> commands;	
	// Read command from stdin
	getline(cin,cmd); 
	// Trim extra whitespaces from command
	cmd = trim(cmd);  
	//Check external command is to be ran in background or nots
        if( cmd.back() == '&')  background = true, cmd.back() = ' ';
	//Split multiple commands (using '\' delimeter) and store each command in array 
	commands = splitCommand(cmd, '|');
	//Return the array of commands
	return commands;
}
//---------------------------------------------------------------------------------------------------------------------------------
// Function to Split the command into input and ouput
vector<string> split_Redirection(string cmd)
{
    
    vector<string> res(3);
    vector< string > splitByOutput = splitCommand(cmd,'>');
    vector< string > splitByInput  = splitCommand(cmd,'<');
    
    //There are four cases of input ouput redirections
    
    // Case 1  : Input Redirection -> YES,  Output Redirection -> NO
    // Example : sort<input.txt
    if(splitByInput.size()==2 && splitByOutput.size()==1){

	res[0] = trim(splitByInput[0]);  //Command -> sort
	res[1] = trim(splitByInput[1]);  //Input   -> input.txt
	return res;
    } 
    
    // Case 2 : Input Redirection -> NO,  Output Redirection -> YES
    // Example : ./a.out > outfile.txt
    if(splitByInput.size()==1 && splitByOutput.size()==2){

	res[0] = trim(splitByOutput[0]); //Command -> ./a.out
	res[2] = trim(splitByOutput[1]); //Output  -> output.txt
	return res;
    }

    // Case 3 : Input Redirection -> YES,  Output Redirection -> YES
    // Example : sort<input.txt>outfile.txt
    if(splitByInput.size()==2 && splitByOutput.size()==2){

	res[0] = trim(splitByInput[0]);  		        //Command -> sort
	res[1] = trim((splitCommand(splitByInput[1],'>'))[0]);  //Input   -> input.txt
	res[2] = trim(splitByOutput[1]);                        //Output  -> output.txt
	return res;
    }

    // Case 4 : Input Redirection -> NO,  Output Redirection -> NO
    // Example : ls
	res[0] = trim(splitByInput[0]); 
    	return res;
}

//---------------------------------------------------------------------------------------------------------------------------------
// Function to open files and redirect input and output with files as arguments
void Redirect_IO(string inputFile, string outputFile)
{
    int inputFD, outputFD;

    // Open input file for redirecting
    if(inputFile.size())
    {
	
        inputFD = open(inputFile.c_str(),O_RDONLY); // Opens file in Read Only Mode and inputFD holds file descriptor number of file -> inputFile

	//Check file opening        
	if(inputFD < 0)
        {
            cout<<"ERROR: Input File could not be opened."<<endl;
            exit(EXIT_FAILURE);
        }
        
	// Redirect input
        if( dup2(inputFD,STDIN_FILENO) < 0 ) //STDIN_FILENO = 0 -> FD of STDIN)
        {
            cout<<"ERROR: Input File could not be redirected."<<endl;
            exit(EXIT_FAILURE);
        }
    }

    // Open output file for redirecting
    if(outputFile.size())
    {
        outputFD = open(outputFile.c_str(), O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);  //outputFD holds file descriptor number of file -> outputFile
					// OCREAT   flag will create file if it does not exists
					// O_WRONLY flag will open file (if it exists) in Write Only Mode
					// O_TRUNC  flag will open file (if it exists) in Truncate Mode
					// S_IRWXU  flag gives permission for read, write, execute/search by owner
	//Check file opening        
	if(outputFD < 0)
        {
            cout<<"ERROR: Output File could not be opened."<<endl;
            exit(EXIT_FAILURE);
        }

        // Redirect output
        if( dup2(outputFD,STDOUT_FILENO) < 0 ) //STDOUT_FILENO = 1 -> FD of STDOUT)
        {
            cout<<"ERROR: Output File could not be redirected."<<endl;
            exit(EXIT_FAILURE);
        }
    }
}

// Function to Execute the commands using syscalls
void executeCommand(string cmd)
{
    // Split the command and its arguments
    vector<string> args = splitCommand(cmd, ' ');
    //First element will be command
    string command = args[0];   
    // Create a char* array for the arguments
    char* Arguments[args.size()+1];
    // Populate array with arguments
    for(int i=0 ; i<args.size() ; i++)  Arguments[i] = const_cast<char*> (args[i].c_str()); // Tyecast it to char *
    // Terminate array with NULL pointer
    Arguments[args.size()] = NULL;             
    // Copy contents to a constant array
    char* const* constArguments = Arguments;
    // Call execvp command     
    if (execvp(command.c_str(),constArguments) < 0)
    {
        fprintf(stderr, "%s: Command not found\n", command.c_str());
    }
}

//---------------------------------------------------------------------------------------------------------------------------------
int main(){

	cout<<"\n*********************************************************************************************************************************\n";
	cout<<"\n";
	cout<<"                  				WELCOME TO OUR SHELL!\n";
	cout<<"               			| Abhinav Bohra 18CS30049 | & | Animesh Jain 18CS10004 |\n";
	cout<<"\n";
	cout<<"*********************************************************************************************************************************\n\n";
	
	int status = 0;	
	char cwd[128], machine[128], user[128], prompt[128]="$";

    	while(1)
    	{

		getlogin_r(user, 128);
		strcpy(machine, "OurShell");
	    getcwd(cwd, 128);
		// flag for background running
		bool background = false; 

		//Display Current Directory Name
		cout<<user<<"@"<<machine<<":~/"<<cwd<<prompt<<" ";
		// Read input command
        	vector< string > cmds = readCommand(background);   
        	// Split the command and its arguments
        	vector<string> args = splitCommand(cmds[0], ' ');

        	// Exit the shell
		if (!(strcmp(args[0].c_str(), "exit") && strcmp(args[0].c_str(), "quit")))
		{
		    cout<<"Thank you, closing shell!\n";
		    exit(0);
		}  
		// Changing directory when "cd" command is used
		if (!(strcmp(args[0].c_str(), "cd")))
		{
		
 		    if (args[1].c_str() == NULL) 
		    {
		        cout<<"Error : expected argument to \"cd\"\n";
		        continue;
		    } 
		    else 
		    {
			
			int flag = chdir(args[1].c_str());		        
			//cout<<args[1].c_str()<<endl<<"Value of chdir: "<<flag<<endl;
			if (flag == -1) 
		        {
		            cout<<"Error : No such directory :"<<args[1].c_str()<<"\n";
		            
		        }
			continue;
		    }
		}   
			
		// CASE 1 : Single Command (0 Pipes)
		if(cmds.size()==1)
		{
		    // Split the commands and redirection
		    vector< string > CIO = split_Redirection(cmds[0]);
		    
		    pid_t pid = fork();
		
		    //IN CHILD PROCESS
		    if(pid == 0) 
		    {
		        Redirect_IO(CIO[1],CIO[2]); // Redirect input and output if required
		        executeCommand(CIO[0]);     // Execute the command
		        exit(0);                    // Exit the child process
		    }
	            //IN PARENT PRROCESS
		    if( pid > 0){
			  
		  	  if(background == false)  wait(&status); //Parent calls the function wait(), thereby waiting for the child to terminate
		    }
		    //FORKING ERROR if pid < 0
		    else perror("Forking Error");
		}

		// CASE 2 : Multiple Commands (n-1 Pipes)

		else
		{
		    int n=cmds.size(),prevFD[2], nextFD[2];

		    for(int i=0; i<n; i++) // Iterate through all the commands
		    {
		        vector<string> CIO = split_Redirection(cmds[i]);
		        
		        //Create new pipe with child file Descriptor if it is not the last command                 
			//Because we don't need piping in last command			
			if(i != n-1){       

				if( pipe(nextFD) == -1){
			        	cout<<"An error occurred while opening the pipe.\n";
					exit(0);
				}
			}

		        pid_t pid = fork();          // Fork each command
	      		
			//IN CHILD PROCESS (Next Process)
		    	if(pid == 0)
		        {
			    //For the first and last command redirect the input output files
		            if( i==0 || i==n-1) Redirect_IO(CIO[1], CIO[2]);  
		            
			    // Read from previous command for all commands except the first command
		            if(i) {
					dup2(prevFD[0],STDIN_FILENO);  
					close(prevFD[0]);
					close(prevFD[1]);
			    }
		           
			    // Write into pipe for everything except last command
		            if(i != n-1){

					close(nextFD[0]);
					dup2(nextFD[1],STDOUT_FILENO);
				    close(nextFD[1]);
			    }
		            
			    // Execute command
		            executeCommand(CIO[0]);
		        }

			//IN PARENT PRROCESS (Prev Process)
		    	if(pid > 0) 
			{                 
			    if(i){
					close(prevFD[0]);
					close(prevFD[1]);
			    }
		             // Copy nextFD into prevFD for all commands except the last command
		            if(i != n-1){
					 prevFD[0] = nextFD[0];
					 prevFD[1] = nextFD[1];
			    }
			}
                        //FORKING ERROR if pid < 0
			else perror("Forking Error");

		    } 

		    if(background == false)  while( wait(&status) > 0); 
		    // If no background running, then wait for all child processes to return
	     	 }
	}
	
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------
