#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define SALT_LENGTH 16

// Estructura para almacenar la información del usuario (nombre de usuario y
// contraseña)
typedef struct
{
    char username[MAX_USERNAME_LENGTH];
    unsigned char password_hash[SHA256_DIGEST_LENGTH];
    unsigned char salt[SALT_LENGTH];
} User;

// Función para generar una contraseña hash utilizando SHA-256 y salting
void hashPassword(const char *password, unsigned char *salt,
                  unsigned char *hash)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, salt, SALT_LENGTH);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(hash, &sha256);
}

// Función para crear un nuevo usuario y almacenarlo en un archivo
void createUser(const char *filename, const char *username, const char *password)
{
    FILE *file = fopen(filename, "ab+");
    if (file == NULL)
    {
        perror("Error al abrir el archivo de usuarios");
        return;
    }

    User user;
    strncpy(user.username, username, MAX_USERNAME_LENGTH);

    // Generar una sal aleatoria para el usuario
    RAND_bytes(user.salt, SALT_LENGTH);

    // Calcular el hash de la contraseña y almacenarlo en user.password_hash
    hashPassword(password, user.salt, user.password_hash);

    // Escribir el usuario en el archivo
    fwrite(&user, sizeof(User), 1, file);
    fclose(file);
}

int findUser(char *filename, const char *username)
{
    FILE *file = fopen(filename, "ab+");
    if (file == NULL)
    {
        perror("Error al abrir el archivo de usuarios");
        return 0;
    }
    User user;
    rewind(file); // Reiniciar el puntero de archivo al principio

    while (fread(&user, sizeof(User), 1, file) == 1)
    {
        if (strcmp(user.username, username) == 0)
        {
            return 1; // El usuario existe
        }
    }
    fclose(file);
    return 0; // El usuario no existe
}

// Función para verificar las credenciales del usuario
int checkCredentials(char *filename, const char *username,
                     const char *password)
{
    FILE *file = fopen(filename, "ab+");

    if (file == NULL)
    {
        perror("Error al abrir el archivo de usuarios");
        return 0;
    }

    User user;

    while (fread(&user, sizeof(User), 1, file) == 1)
    {
        if (strcmp(user.username, username) == 0)
        {
            unsigned char test_hash[SHA256_DIGEST_LENGTH];
            hashPassword(password, user.salt, test_hash);

            if (memcmp(user.password_hash, test_hash, SHA256_DIGEST_LENGTH) == 0)
            {
                return 1; // Las credenciales son válidas
            }
        }
    }

    fclose(file);

    return 0; // Las credenciales son incorrectas
}

/* void auth()
{
    char *filename = "users.dat";
    int opcion;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("1. Crear usuario\n2. Iniciar sesión\nSeleccione una opción: ");
    scanf("%d", &opcion);

    if (opcion == 1)
    {
        printf("Nombre de usuario: ");
        scanf("%s", username);
        if (findUser(filename, username))
        {
            printf("El nombre de usuario ya existe.\n");
        }
        else
        {
            char *password = getpass("Contraseña: ");
            createUser(filename, username, password);
            printf("Usuario creado con éxito.\n");
        }
    }
    else if (opcion == 2)
    {
        printf("Nombre de usuario: ");
        scanf("%s", username);
        char *password = getpass("Contraseña: ");

        if (checkCredentials(filename, username, password))
        {
            printf("Inicio de sesión exitoso.\n");
        }
        else
        {
            printf("Inicio de sesión fallido.\n");
        }
    }
    else
    {
        printf("Opción no válida.\n");
    }
}
 */