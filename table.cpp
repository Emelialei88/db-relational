#include <iostream>
#include <sstream>
#include "types.h"
#include "page.cpp"

class Table {
public:
	Table(const char* filename) {
		pager = new Pager(filename);
	}
	~Table() {
		delete pager;
	}

	Row* get_row(int row_num) {
		int page_num = row_num / ROWS_PER_PAGE;
		Page* page = pager->get_page(page_num);
		uint32_t row_offset = row_num % ROWS_PER_PAGE;
		return page->get_row(row_offset);
	}

	ExecuteResult insert(Row* row) {
		if (pager->num_rows >= TABLE_MAX_ROWS) {
			return ExecuteResult::TABLEFULL;
		}
		Row* des_row = get_row(pager->num_rows);
		memcpy(&(des_row->id), &(row->id), sizeof(row->id));
		memcpy(des_row->username, row->username, COLUMN_USERNAME_SIZE);
		memcpy(des_row->email, row->email, COLUMN_EMAIL_SIZE);
		++pager->num_rows;
		return ExecuteResult::SUCCESS;
	}

	ExecuteResult select() {
		if (pager->num_rows == 0) {
			cout << "Empty database";
			return ExecuteResult::SUCCESS;
		}
		for (int i = 0; i < pager->num_rows; ++i) {
			Row* row = get_row(i);
			cout << "(" << row->id << ", " << row->username << ", " << row->email << ")" << endl;
		}		
		return ExecuteResult::SUCCESS;
	}

private:
	Pager* pager;
};