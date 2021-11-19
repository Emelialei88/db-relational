#include "row.cpp"

class Page {
public:
	Page() {
		rows = new Row*[ROWS_PER_PAGE];
		for (int i = 0; i < ROWS_PER_PAGE; ++i) {
			rows[i] = new Row();
		}
	}
	~Page() {
		for (int i = 0; i < ROWS_PER_PAGE; ++i) {
			delete rows[i];
		}
		delete [] rows;
	}

	Row* get_row(int row) {
		return rows[row];
	}
private:
	Row** rows;
};