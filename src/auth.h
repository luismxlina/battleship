#ifndef AUTH_H
#define AUTH_H

#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define SALT_LENGTH 16

typedef struct {
  char username[MAX_USERNAME_LENGTH];
  unsigned char password_hash[SHA256_DIGEST_LENGTH];
  unsigned char salt[SALT_LENGTH];
} User;

void hash_password(const char *password, unsigned char *salt,
                   unsigned char *hash);
void crear_usuario(FILE *file, const char *username, const char *password);
int buscar_usuario(FILE *file, const char *username);
int verificar_credenciales(FILE *file, const char *username,
                           const char *password);
void auth();

#endif /* AUTH_H */