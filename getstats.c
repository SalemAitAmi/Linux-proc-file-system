#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void print_process_info(char* pid) {
    char proc[64] = "/proc/";  // Path to proc file system
    char stat[64];             // Holds filename of "/proc/PID/status"
    char cmd[64];              // Holds filename of "/proc/PID/cmdline"
    char line[512];            // Buffer for reads
    int threads = 0;           // Threads count
    int v_ctxt_sw = 0;         // Voluntary context swicth count
    int nonv_ctxt_sw = 0;      // Non-voluntary context switch count

    // Construct filenames
    strcat(proc, pid);              
    strncpy(stat, proc, sizeof(proc));  
    strncpy(cmd, proc, sizeof(proc));
    strcat(cmd, "/cmdline");
    strcat(stat, "/status");
    
    // File pointers
    FILE* status = fopen(stat, "r");
    FILE* cmdline = fopen(cmd, "r");
    
    if (status == NULL){
        printf("process number %s not found.\n", pid);
        exit(1);
    }
    
    // First line of status holds process name
    fgets(line, sizeof(line), status);

    // Print process number and name
    printf("Process number: %s\n", pid);
    printf("%s", line);
    
    // If a command line was used to launch the process, retrieve it along with its arguments
    // NOTE: Parsing is done this way because arguments are separated by NULL characters instead of spaces
    if(cmdline != NULL){
        int c;
        printf("Command Line: ");
        while(1){
            c = fgetc(cmdline);
            if(feof(cmdline)){
                printf("\n");
                break;
            }
            printf("%c", c);
        }
    }
    
    // Buffers key-value pairs
    char key[64];
    char value[64];

    // Read status file line by line 
    while (fgets(line, sizeof(line), status)) {
        sscanf(line, "%s %s\n", key, value);
        
        if (strcmp(key, "Threads:") == 0) {
            sscanf(value, "%d", &threads);
        }
        else if (strcmp(key, "voluntary_ctxt_switches:") == 0) {
            sscanf(value, "%d", &v_ctxt_sw);
        }
        else if (strcmp(key, "nonvoluntary_ctxt_switches:") == 0) {
            sscanf(value, "%d", &nonv_ctxt_sw);
        }
    }
    
    printf("Threads: %d\n", threads);
    printf("Total Context Switches: %d\n", v_ctxt_sw+nonv_ctxt_sw);
    
    fclose(status);
    fclose(cmdline);
} 


void print_full_info() {
    FILE* cpuinfo = fopen("/proc/cpuinfo", "r");  // Contains information about the CPU on the system
    FILE* version = fopen("/proc/version", "r");  // Contains information about the currently running kernel 
    FILE* meminfo = fopen("/proc/meminfo", "r");  // Contains information about the system's memory usage
    FILE* uptime = fopen("/proc/uptime", "r");    // Contains two values: the total uptime of the system and the time spent in an idle state
    char line[256];                               // Buffer for reads
    int lcount = 26;                              // # of lines until core 1 information (i.e. general info repeats for each core)
    

    while(lcount != 0){
        fgets(line, sizeof(line), cpuinfo);
        if(lcount == 14 || lcount == 22){
        printf("%s", line);
        }
        lcount--;
    }
    
    // Info in version is all contained on one line
    fgets(line, sizeof(line), version);
    printf("%s", line);

    // Only require the first line in meminfo, which contains the total memory available to the system
    fgets(line, sizeof(line), meminfo);
    printf("%s", line);
    
    float up_totalsec = 0;

    // First value in uptime specifies how long the system has been up in seconds (i.e. The second value is the sum of how much time each core has spent idle)
    fscanf(uptime, "%f ", &up_totalsec);
    int days = up_totalsec/86400; 
    up_totalsec -= days*86400;

    int hours = up_totalsec/3600; 
    up_totalsec -= hours*3600;

    int minutes = up_totalsec/60; 
    up_totalsec -= minutes*60;

    int seconds = (int) up_totalsec;

    printf("Uptime: %d days, %d hours, %d minutes, %d seconds\n", days, hours, minutes, seconds);
    
    fclose(cpuinfo);
    fclose(version);
    fclose(meminfo);
    fclose(uptime);
}


int main(int argc, char ** argv) {  
    if (argc == 1) {
        print_full_info();
    } 
    else if (argc == 2){
        print_process_info(argv[1]);
    }
    else {
        printf("Invalid input, too many arguments!\n");
    }

    return 0;
}
