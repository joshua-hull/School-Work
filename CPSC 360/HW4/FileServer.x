#define VERSION_NUMBER 1
#define PROGRAM_NUMBER 22222222

program FILESERVER {
   version SIMP_VERSION {
     string getFile(string) = 1;
   } = VERSION_NUMBER;
} = PROGRAM_NUMBER;