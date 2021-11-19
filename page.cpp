#include <iostream>
#include <fstream>
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

class Pager {
public:
	Pager(const char* filename): fs(filename) {
		num_rows = 0;
		if (!fs.is_open()) {
			perror("error while opening file -- not opened");
			exit(EXIT_FAILURE);
		}
		fs.seekg(0, fs.end);
		file_length = fs.tellg();
		for (int i = 0; i < TABLE_MAX_PAGES; ++i) {
			pages[i] = nullptr;
		}
	}

	~Pager() {
		uint32_t num_full_pages = num_rows / PAGE_SIZE;
		for (uint32_t i = 0; i < num_full_pages; ++i) {
			if (pages[i] == nullptr) {
				continue;
			}
			flush(i, PAGE_SIZE);
			delete pages[i];
			pages[i] = nullptr;
		}
		// There might be a partial page at the end of file
		uint32_t additional_rows = num_rows % PAGE_SIZE;
		if (additional_rows > 0) {
			uint32_t page_num = num_full_pages;
			if (pages[page_num] != nullptr) {
				flush(page_num, additional_rows * ROW_SIZE);
				delete pages[page_num];
				pages[page_num] = nullptr;
			}
		}
		// Closs filestream
		fs.close();
		// A final thorough scan
		for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
			Page* page = pages[i];
			if (page) {
				delete page;
				pages[i] = nullptr;
			}
		}
	}

	void flush(uint32_t page_num, int size) {
		if (pages[page_num] == nullptr) {
			perror("error trying to flush null page.");
			exit(EXIT_FAILURE);
		}
		//fs.seekg(page_num * PAGE_SIZE, ios::beg);
		fs.seekg(0, ios::beg);
		fs.write((char*)pages[page_num]->get_row(0), size);
		// TODO: error here
		if (fs.bad()) {
			perror("error while writing file"); 
			exit(EXIT_FAILURE);
		}
	}

	Page* get_page(int page_num) {
		if (page_num > TABLE_MAX_PAGES) {
			perror("Error: Tried to fetch page number out of bounds.");
			cout << page_num << " < " << TABLE_MAX_PAGES << endl;
			exit(EXIT_FAILURE);
		}
		if (pages[page_num] == nullptr) {
			Page* page = new Page();
			uint32_t num_pages = file_length / PAGE_SIZE;

			// There might be a partial page at the end of file
			if (file_length % PAGE_SIZE) {
				num_pages++;
			}

			// Fetch page from file into page
			if (page_num <= num_pages) {
				fs.seekg(page_num * PAGE_SIZE, ios::beg);
				//char* page_tmp = (char*)malloc(PAGE_SIZE);
				//fs.read(page_tmp, PAGE_SIZE);
				fs.read((char*)(page->get_row(0)), PAGE_SIZE);
				// TODO: error here
				if (fs.bad()) {
					perror("error while reading file");
					exit(EXIT_FAILURE);
				}
			}
			pages[page_num] = page;
		}
		return pages[page_num];
	}

	fstream fs;
	uint32_t file_length;
	uint32_t num_rows;
	Page* pages[TABLE_MAX_PAGES];
};