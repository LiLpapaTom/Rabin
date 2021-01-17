//Thomas Papaloukas, icsd14155
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

gmp_randstate_t stat;

int main(void){
	mpz_t p,q,n,plain,cipher,seed,m,root,tmp,m1,m2,m3,m4,p_1,c,q_1;
	char str[100]={'0'},buff[100]={'0'},array[2];
	long sd=0;
	int i,j=0;
	unsigned long int ascii[100];

	gmp_randinit(stat, GMP_RAND_ALG_LC, 120);
	mpz_init(p);
	mpz_init(q);
	mpz_init(n);
	mpz_init(seed);
	mpz_init(plain);
	mpz_init(cipher);
	mpz_init(m);
	mpz_init(root);
	mpz_init(tmp);
	mpz_init(m1);
	mpz_init(m2);
	mpz_init(m3);
	mpz_init(m4);
	mpz_init(p_1);
	mpz_init(c);
	mpz_init(q_1);

	srand( (unsigned) getpid() );
	sd = rand();
	mpz_set_ui(seed, sd);

	gmp_randseed(stat, seed);

	mpz_urandomb(p, stat, 200);
	mpz_urandomb(q, stat, 200);

	printf("Please enter your message:\n");
	fgets(str, 100, stdin);
	
	printf("Text to ascii:\n");
	for(i=0;i<strlen(str);i++){
		ascii[i] = (unsigned long int) str[i];
		printf("%ld",ascii[i]);
	}
	puts("");
	for(i=0;i<strlen(str)*2;i++){
		sprintf(&buff[i], "%ld", ascii[j]/10);
		sprintf(&buff[i+1], "%ld", ascii[j]%10);
		j++;
	}
	//puts(buff);
	mpz_set_str(plain, buff, 10);
	gmp_printf("Plaintext: %Zd\n",plain);

	//Find primes p,q based on probability functions
	j=0; //temp
	while(j==0){
		mpz_urandomb(p, stat, 200);
		//Break out of while, if p is prime, and p congruents  3 mod 4
		if( !mpz_probab_prime_p(p,10) && !mpz_congruent_ui_p(p,3,4) )
			j=1;
	}
	gmp_printf("p = %Zd\n",p);
	j=0;
	while(j==0){
		mpz_urandomb(q, stat, 200);
		if( !mpz_probab_prime_p(q,10) && !mpz_congruent_ui_p(q,3,4) )
			j=1;
	}
	gmp_printf("q = %Zd\n",q);

	mpz_mul(n, p, q);
	gmp_printf("n = %Zd\n",n);

	//Encrypt
	mpz_mul(m, plain, plain);
	mpz_mod(cipher, m, n);
	gmp_printf("Ciphertext: %Zd\n", cipher);

	//Decrypt
	mpz_root(root, cipher, 2);
	gmp_printf("%Zd\n",root);
	//mpz_mod(tmp, root, n);
	//gmp_printf("%Zd\n",tmp);
	
	//From Euler's criterion we claim that
	//c ^ (p - 1)/4 congruents 1 mod p
	//hence the 4 roots are :
	//c ^ (p-1)/4 mod p , - ( c ^ (p-1)/4 mod p ),  
	//c ^ (q-1)/4 mod q , - ( c ^ (q-1)/4 mod p ). 
	//root 1
	mpz_add_ui(p_1, p, 1);       //p_1 = p + 1
	mpz_div_ui(p_1, p_1, 4);     //p_1 = (p+1) / 4
	mpz_powm(m1, cipher, p_1, p);//m1 = c^(p+1)/4 mod p
	//gmp_printf("m1 = %Zd\n",m1);

	//root 2
	mpz_add_ui(p_1, p, 1);	     //p_1 = p + 1
	mpz_div_ui(p_1, p_1, 4);     //p_1 = (p+1) / 4
	mpz_mul_ui(c, cipher, -1);   //c = cipher * (-1), the negative root
	mpz_powm(m2, c, p_1, p);     //m2 = c^(p+1)/4 mod p
	//gmp_printf("m2 = %Zd\n",m2);

	//root 3
	mpz_add_ui(q_1, q, 1);
	mpz_div_ui(q_1, q_1, 4);
	mpz_powm(m3, cipher, q_1, q);
	//gmp_printf("m3 = %Zd\n",m3);

	//root 4
	mpz_add_ui(q_1, q, 1);
	mpz_div_ui(q_1, q_1, 4);
	mpz_powm(m4, c, q_1, q);
	//gmp_printf("m4 = %Zd\n",m4);
	
	mpz_mod(m1, m1, n);
	mpz_mod(m2, m2, n);
	mpz_mod(m3, m3, n);
	mpz_mod(m4, m4, n);
	puts("Possible plaintexts are: ");
	gmp_printf("m1 = %Zd\n",m1);
	gmp_printf("m2 = %Zd\n",m2);
	gmp_printf("m3 = %Zd\n",m3);
	gmp_printf("m4 = %Zd\n",m4);
	
	//mpz_powm(decrypted, cipher, d, n);
	//gmp_printf("Decrypter Text : %Zd\n",decrypted);
	//("");
	//pz_get_str(buff, 10, decrypted);	
	
	//Plaing text to chars
	mpz_get_str(buff, 10, m1);		
	j=0;
	for(i=0;i<strlen(buff);i+=2){
	  array[0] = buff[i];
	  array[1] = buff[i+1];
	  ascii[j] = atoi(array);
	  j++;
	}
	i=0;
	do{
		str[i] = (char) ascii[i];
		i++;
	}while(!ascii[i]);
	puts("Root m1 to plaintext");
	puts(str);
	
	//Root 2 to plain
	mpz_get_str(buff, 10, m2);		
	j=0;
	for(i=0;i<strlen(buff);i+=2){
	  array[0] = buff[i];
	  array[1] = buff[i+1];
	  ascii[j] = atoi(array);
	  j++;
	}
	i=0;
	do{
		str[i] = (char) ascii[i];
		i++;
	}while(!ascii[i]);
	puts("Root m2 to plaintext");
	puts(str);
	
	//Root 3 to plain
	mpz_get_str(buff, 10, m3);		
	j=0;
	for(i=0;i<strlen(buff);i+=2){
	  array[0] = buff[i];
	  array[1] = buff[i+1];
	  ascii[j] = atoi(array);
	  j++;
	}
	i=0;
	do{
		str[i] = (char) ascii[i];
		i++;
	}while(!ascii[i]);
	puts("Root m3 to plaintext");
	puts(str);
	
	//Root 4 to plain
	mpz_get_str(buff, 10, m4);		
	j=0;
	for(i=0;i<strlen(buff);i+=2){
	  array[0] = buff[i];
	  array[1] = buff[i+1];
	  ascii[j] = atoi(array);
	  j++;
	}
	i=0;
	do{
		str[i] = (char) ascii[i];
		i++;
	}while(!ascii[i]);
	puts("Root m4 to plaintext");
	puts(str);
}
