#define VERSION_NUMBER 1
#define PROGRAM_NUMBER 55555555

struct register_operand {
	string name<128>;
	string hostname<128>;
};

program NAMESERVER {
   version SIMP_VERSION {
     int registerServer(register_operand) = 1;
     int removeServer(string) = 2;
     string getServerHostName(string) = 3;
   } = VERSION_NUMBER;
} = PROGRAM_NUMBER;