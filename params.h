#pragma once
// row
#define COLUMN_USERNAME_SIZE 33
#define COLUMN_EMAIL_SIZE 256
#define ROW_SIZE 10 // TODO: modify
// page
#define PAGE_SIZE 4096
#define ROWS_PER_PAGE (PAGE_SIZE / ROW_SIZE)
// table
#define TABLE_MAX_PAGES 100
#define TABLE_MAX_ROWS (ROWS_PER_PAGE * TABLE_MAX_PAGES)