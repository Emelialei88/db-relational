#include <iostream>
#include <sstream>
#include "types.h"
#include "page.cpp"

class Table {
public:
	Table() {
		num_rows = 0;
		for (int i = 0; i < TABLE_MAX_PAGES; ++i) {
			pages[i] = nullptr;
		}
	}

	Page* get_page(int row_num) {
		int page_num = row_num / ROWS_PER_PAGE;
		Page* page = pages[page_num];
		if (page == nullptr) {
			page = pages[page_num] = new Page();
		}
		return page;
	}

	Row* get_row(int row_num) {
		Page* page = get_page(row_num);
		uint32_t row_offset = row_num % ROWS_PER_PAGE;
		return page->get_row(row_offset);
	}

	ExecuteResult insert(Row* row) {
		if (num_rows >= TABLE_MAX_ROWS) {
			return ExecuteResult::TABLEFULL;
		}
		Row* des_row = get_row(num_rows);
		memcpy(&(des_row->id), &(row->id), sizeof(row->id));
		memcpy(des_row->username, row->username, COLUMN_USERNAME_SIZE);
		memcpy(des_row->email, row->email, COLUMN_EMAIL_SIZE);
		++num_rows;
		return ExecuteResult::SUCCESS;
	}

	ExecuteResult select() {
		if (num_rows == 0) {
			cout << "Empty database";
			return ExecuteResult::SUCCESS;
		}
		for (int i = 0; i < num_rows; ++i) {
			Row* row = get_row(i);
			cout << "(" << row->id << ", " << row->username << ", " << row->email << ")" << endl;
		}		
		return ExecuteResult::SUCCESS;
	}

private:
	uint32_t num_rows;
	Page* pages[TABLE_MAX_PAGES];
};