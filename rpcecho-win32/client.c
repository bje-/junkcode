/* 
   RPC echo client.

   Copyright (C) Tim Potter 2003
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "rpcecho.h"

void main(int argc, char **argv)
{
	RPC_STATUS status;
	char *binding = NULL;
	const char *username=NULL;
	const char *password=NULL;
	const char *domain=NULL;
	unsigned sec_options = 0;

	argv += 1;
	argc -= 1;

	while (argc > 2 && argv[0][0] == '-') {
		const char *option;

		switch (argv[0][1]) {
		case 'e':
			binding = argv[1];
			break;
		case 'u':
			username = argv[1];
			break;
		case 'p':
			password = argv[1];
			break;
		case 'd':
			domain = argv[1];
			break;
		case '-':
			option = &argv[0][2];
			if (strcmp(option, "sign") == 0) {
				sec_options |= RPC_C_AUTHN_LEVEL_PKT_INTEGRITY;
			} else if (strcmp(option, "seal") == 0) {
				sec_options |= RPC_C_AUTHN_LEVEL_PKT_PRIVACY;
			} else {
				printf("Bad security option '%s'\n", option);
				exit(1);
			}
			argv++;
			argc--;
			continue;
		default:
			printf("Bad option -%c\n", argv[0][0]);
			exit(1);
		}
		argv += 2;
		argc -= 2;
	}

	if (argc < 2) {
		printf("Usage: client [options] hostname cmd [args]\n\n");
		printf("Where hostname is the name of the host to connect to,\n");
		printf("and cmd is the command to execute with optional args:\n\n");
		printf("\taddone num\tAdd one to num and return the result\n");
		printf("\techodata size\tSend an array of size bytes and receive it back\n");
		printf("\tsinkdata size\tSend an array of size bytes\n");
		printf("\tsourcedata size\tReceive an array of size bytes\n");
		printf("\ttest\trun testcall\n");
		printf("\noptions:\n");
		printf("\t-u username -d domain -p password -e endpoint\n");
		printf("\t--sign --seal\n");
		printf("\nExamples:\n");
		printf("\tclient HOSTNAME addone 3\n");
		printf("\tclient 192.168.115.1 addone 3\n");
		printf("\tclient -e ncacn_np:HOSTNAME[\\\\pipe\\\\rpcecho] addone 3\n");
		printf("\tclient -e ncacn_ip_tcp:192.168.115.1 addone 3\n");
		printf("\tclient -e ncacn_ip_tcp:192.168.115.1 -u tridge -d MYDOMAIN -p PASSWORD addone 3\n");
		exit(0);
	}


	if (!binding) {
		char *network_address = argv[0];

		argc--;
		argv++;

		status = RpcStringBindingCompose(
			NULL, /* uuid */
			"ncacn_np",
			network_address,
			"\\pipe\\rpcecho",
			NULL, /* options */
			&binding);

		if (status) {
			printf("RpcStringBindingCompose returned %d\n", status);
			exit(status);
		}
	}

	printf("Endpoint is %s\n", binding);

	status = RpcBindingFromStringBinding(
			binding,
			&rpcecho_IfHandle);

	if (status) {
		printf("RpcBindingFromStringBinding returned %d\n", status);
		exit(status);
	}

	if (username) {
		SEC_WINNT_AUTH_IDENTITY ident = { username, strlen(username),
						  domain, strlen(domain),
						  password, strlen(password),
						  SEC_WINNT_AUTH_IDENTITY_ANSI };

		status = RpcBindingSetAuthInfo(rpcecho_IfHandle, NULL,
					       sec_options,
					       RPC_C_AUTHN_WINNT,
					       &ident, 0);
		if (status) {
			printf ("RpcBindingSetAuthInfo failed: 0x%x\n", status);
			exit (1);
		}
	}


	RpcTryExcept {

		/* Add one to a number */

		if (strcmp(argv[0], "addone") == 0) {
			int arg, result;

			if (argc != 2) {
				printf("Usage: addone num\n");
				exit(1);
			}

			arg = atoi(argv[1]);

			AddOne(arg, &result);
			printf("%d + 1 = %d\n", arg, result);

			goto done;
		}

		/* Echo an array */

		if (strcmp(argv[0], "echodata") == 0) {
			int arg, i;
			char *indata, *outdata;

			if (argc != 2) {
				printf("Usage: echo num\n");
				exit(1);
			}

			arg = atoi(argv[1]);

			if ((indata = malloc(arg)) == NULL) {
				printf("Error allocating %d bytes for input\n", arg);
				exit(1);
			}

			if ((outdata = malloc(arg)) == NULL) {
				printf("Error allocating %d bytes for output\n", arg);
				exit(1);
			}

			for (i = 0; i < arg; i++)
				indata[i] = i & 0xff;

			EchoData(arg, indata, outdata);

			for (i = 0; i < arg; i++) {
				if (indata[i] != outdata[i]) {
					printf("data mismatch at offset %d, %d != %d\n",
						i, indata[i], outdata[i]);
					exit(0);
				}
			}

			goto done;
		}

		if (strcmp(argv[0], "sinkdata") == 0) {
			int arg, i;
			char *indata;

			if (argc != 2) {
				printf("Usage: sinkdata num\n");
				exit(1);
			}

			arg = atoi(argv[1]);		

			if ((indata = malloc(arg)) == NULL) {
				printf("Error allocating %d bytes for input\n", arg);
				exit(1);
			}			

			for (i = 0; i < arg; i++)
				indata[i] = i & 0xff;

			SinkData(arg, indata);

			goto done;
		}

		if (strcmp(argv[0], "sourcedata") == 0) {
			int arg, i;
			unsigned char *outdata;

			if (argc != 2) {
				printf("Usage: sourcedata num\n");
				exit(1);
			}

			arg = atoi(argv[1]);		

			if ((outdata = malloc(arg)) == NULL) {
				printf("Error allocating %d bytes for output\n", arg);
				exit(1);
			}			

			SourceData(arg, outdata);

			for (i = 0; i < arg; i++) {
				if (outdata[i] != (i & 0xff)) {
					printf("data mismatch at offset %d, %d != %d\n",
						i, outdata[i], i & 0xff);
				}
			}

			goto done;
		}

		if (strcmp(argv[0], "test") == 0) {
			printf("no TestCall\n");
			goto done;
		}

		printf("Invalid command '%s'\n", argv[0]);

	} RpcExcept(1) {
		unsigned long ex;

		ex = RpcExceptionCode();
		printf("Runtime error 0x%x\n", ex);
	} RpcEndExcept

done:

	status = RpcStringFree(&binding);

	if (status) {
		printf("RpcStringFree returned %d\n", status);
		exit(status);
	}

	status = RpcBindingFree(&rpcecho_IfHandle);

	if (status) {
		printf("RpcBindingFree returned %d\n", status);
		exit(status);
	}

	exit(0);
}
