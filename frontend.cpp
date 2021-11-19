#include <iostream>
#include <string>
#include "table.cpp"
#include "types.h"

using namespace std;

class Buffer {
public:
	Buffer() {
		row = new Row();
		table = new Table();
		input = "";
		type = StatementType::STATEMENT_DEFAULT;
	}
	~Buffer() {
		delete row;
		delete table;
	}
	void prompt() { cout << "db> "; }
	void read_input() {
		getline(cin, input);
	}
	string get_input() {
		return input;
	}
	MetaCommand do_meta_cmd() {
		if (input == ".exit") {
			exit(EXIT_SUCCESS);
		}
		else {
			return MetaCommand::UNRECOGNIZED;
		}
	}
	PrepareResult prepare_statement() {
		if (input.substr(0, 6) == "insert") {
			type = StatementType::STATEMENT_INSERT;
			int arg_assigned = sscanf_s(input.c_str(), "insert %d %s %s", &(row->id), row->username, COLUMN_USERNAME_SIZE, row->email, COLUMN_EMAIL_SIZE);
			if (arg_assigned < 3) {
				if (!strcmp(row->username, "") || !strcmp(row->email, "")) {
					return PrepareResult::STRING_TOO_LONG;
				}
				return PrepareResult::SYNTAX_ERROR;
			}
			if (row->id < 0) {
				return PrepareResult::NEGATIVE_ID;
			}
			
			return PrepareResult::SUCCESS;
		}
		if (input.substr(0, 6) == "select") {
			type = StatementType::STATEMENT_SELECT;
			return PrepareResult::SUCCESS;
		}
		return PrepareResult::UNRECOGNIZED;
	}
	ExecuteResult execute() {
		switch (type)
		{
		case StatementType::STATEMENT_INSERT:
			return table->insert(row);
		case StatementType::STATEMENT_SELECT:
			return table->select();
		default:
			break;
		}
	}
private:
	string input;
	StatementType type;
	Table* table;
	Row* row;
};

int main() {
	Buffer* buf = new Buffer();
	while (true) {
		buf->prompt();
		buf->read_input();
		string input = buf->get_input();
		// meta command starts with "."
		if (input[0] == '.') {
			MetaCommand meta_cmd = buf->do_meta_cmd();
			switch (meta_cmd)
			{
			case MetaCommand::SUCCESS:
				continue;
			case MetaCommand::UNRECOGNIZED:
				cout << "Unrecognized command '" << input << "'" << endl;
				continue;
			}
		}
		// normal statements start without a "."
		// prepare and parse the input 
		switch (buf->prepare_statement())
		{
		case PrepareResult::SUCCESS:
			break;
		case PrepareResult::SYNTAX_ERROR:
			cout << "Syntax error. Could not parse statement." << endl;
			continue;
		case PrepareResult::STRING_TOO_LONG:
			cout << "Input string is too long" << endl;
			continue;
		case PrepareResult::NEGATIVE_ID:
			cout << "ID must be positive." << endl;
			continue;
		case PrepareResult::UNRECOGNIZED:
			cout << "Unrecognized keyword at start of '" << input << "'" << endl;
			continue;
		}
		// Execute statement
		switch (buf->execute())
		{
		case ExecuteResult::SUCCESS:
			cout << "Executed" << endl;
			break;
		case ExecuteResult::TABLEFULL:
			cout << "Error: Table full." << endl;
			break;
		}
	}
}