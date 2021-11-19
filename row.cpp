#include <stdint.h>
#include <string>
#include "params.h"

using namespace std;

struct Row {
	uint32_t id;
	char username[COLUMN_USERNAME_SIZE];
	char email[COLUMN_EMAIL_SIZE];
};