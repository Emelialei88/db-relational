#pragma once
enum class MetaCommand { SUCCESS, UNRECOGNIZED };
enum class StatementType { STATEMENT_INSERT, STATEMENT_SELECT, STATEMENT_DEFAULT };
enum class PrepareResult { SUCCESS, SYNTAX_ERROR, STRING_TOO_LONG, NEGATIVE_ID, UNRECOGNIZED };
enum class ExecuteResult { SUCCESS, TABLEFULL };