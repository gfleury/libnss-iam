//#include "headers.h" 
#include <pwd.h>

/**
 * test iam users retrieve
 */
#ifdef TEST
#include <string.h>
int main(int argc, char** argv)
{
        char buffer[1024];
        struct passwd p;
	memset(buffer, '0', 1024);
	if (argc>1)
        	strcpy(buffer, argv[1]);

        if(get_posix_iam_user(buffer, 1024, &p)) {
                printf("%s (%d)\n", p.pw_name, p.pw_uid);
		if(get_posix_iam_user_by_uid(13903, buffer, 1024, &p)) 
                	printf("%s (%d)\n", p.pw_name, p.pw_uid);
	}
}
#endif
