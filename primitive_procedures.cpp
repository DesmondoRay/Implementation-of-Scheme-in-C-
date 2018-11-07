/* Implement of primitive procedures of Scheme */

#include "primitive_procedures.h"
#include "eval.h"

/* Quit */
Object Primitive::quit(vector<Object>& obs)
{
	cout << "Bye! Press any key to quit." << endl;
	char input = getchar();
	exit(0);
}

/* Return the sum of obs. */
Object Primitive::add(vector<Object>& obs)
{
	int64_t sum_i = 0;
	double sum_f = 0.0;
	for (auto &ob : obs) {
		if (ob.get_type() == INTEGER)
			sum_i += ob.get_integer();
		else if (ob.get_type() == REAL)
			sum_f += ob.get_real();
		else {
			string error_msg("ERROR(scheme): can't apply '+' to the type -- ");
			error_msg += ob.get_type_str();
			error_handler(error_msg);
		}
	}

	if (sum_f != 0.0)
		return Object(sum_i + sum_f);	// Result's type is REAL
	if (sum_i > INT_MAX || sum_i < INT_MIN)
		cerr << "WARING: overflow -- +, " << sum_i
		<< " --> " << static_cast<int>(sum_i) << endl;
	return Object(static_cast<int>(sum_i));	// Result's type is INTEGER
}

/* Return the difference of obs */
Object Primitive::sub(vector<Object>& obs)
{
	if (obs.empty()) return Object(0);

	int diff_i = (obs[0].get_type() == INTEGER ? obs[0].get_integer() : 0);
	double diff_f = (obs[0].get_type() == REAL ? obs[0].get_real() : 0.0);
	for (int i = 1; i < obs.size(); i++) {
		Object &ob = obs[i];
		if (ob.get_type() == INTEGER)
			diff_i -= ob.get_integer();
		else if (ob.get_type() == REAL)
			diff_f -= ob.get_real();
		else {
			string error_msg("ERROR(scheme): can't apply '-' to the type -- ");
			error_msg += ob.get_type_str();
			error_handler(error_msg);
		}
	}

	if (diff_f != 0.0)
		return Object(diff_f + diff_i);	// Result's type is REAL
	return Object(diff_i);				// Result's type is INTEGER
}

/* Return the product of obs */
Object Primitive::mul(vector<Object>& obs)
{
	int64_t pro_i = 1;
	double pro_f = 1.0;
	for (auto &ob : obs) {
		if (ob.get_type() == INTEGER)
			pro_i *= ob.get_integer();
		else if (ob.get_type() == REAL)
			pro_f *= ob.get_real();
		else {
			string error_msg("ERROR(scheme): can't apply '*' to the type -- ");
			error_msg += ob.get_type_str();
			error_handler(error_msg);
		}
	}

	if (pro_f != 1.0)
		return Object(pro_f * pro_i);	// Result's type is REAL
	if (pro_i > INT_MAX || pro_i < INT_MIN)
		cerr << "WARING: overflow -- *, " << pro_i 
			 << " --> " << static_cast<int>(pro_i) << endl;
	return Object(static_cast<int>(pro_i));	// Result's type is INTEGER
}

/* Return the quotient of obs */
Object Primitive::div(vector<Object>& obs)
{
	if (obs.empty()) return Object(0);

	double quotient = (obs[0].get_type() == REAL ? obs[0].get_real() :
		static_cast<double>(obs[0].get_integer()));

	for (int i = 1; i < obs.size(); i++) {
		Object &ob = obs[i];
		if (ob.get_type() == INTEGER) {
			if (ob.get_integer() == 0)
				error_handler("ERROR(scheme): division by zero");
			quotient /= ob.get_integer();
		}
		else if (ob.get_type() == REAL) {
			if (ob.get_real() == 0.0)
				error_handler("ERROR(scheme): floating-point divide by zero");
			quotient /= ob.get_real();
		}
		else {
			string error_msg("ERROR(scheme): can't apply '/' to the type -- ");
			error_msg += ob.get_type_str();
			error_handler(error_msg);
		}
	}

	return Object(quotient);	// Result's type is REAL
}

/* Return remainder */
Object Primitive::remainder(vector<Object>& obs) 
{
	if (obs.size() < 2) {
		error_handler("ERROR(scheme): remainder takes at least two arguments");
	}
	if (obs[0].get_type() != INTEGER || obs[1].get_type() != INTEGER) {
		string wrong_type = (obs[0].get_type() == INTEGER ?
			obs[1].get_type_str() : obs[0].get_type_str());
		string error_msg("ERROR(scheme): passed a ");
		error_msg += wrong_type;
		error_msg += " to remainder, it only takes integer.";
		error_handler(error_msg);
	}

	return Object(obs[0].get_integer() % obs[1].get_integer());
}

/* Return the true if obs[0] < obs[1] < obs[2] < ... < obs[n] */
Object Primitive::less(vector<Object>& obs)
{
	bool result = true;
	for (int i = 0; i < obs.size() - 1; i++) {
		result &= (obs[i] < obs[i + 1]);
	}
	return Object(result);
}

/* Return the true if obs[0] > obs[1] > obs[2] > ... > obs[n] */
Object Primitive::greater(vector<Object>& obs)
{
	bool result = true;
	for (int i = 0; i < obs.size() - 1; i++) {
		result &= (obs[i] > obs[i + 1]);
	}
	return Object(result);
}

/* Return true if obs[0] == obs[1] == obs[2] == ... == obs[n] */
/* arguments must be numbers */
Object Primitive::op_equal(vector<Object>& obs) {
	if (obs.size() < 2) {
		error_handler("ERROR(scheme): = takes at least two arguments");
	}

	bool result = true;
	for (int i = 0; i < obs.size() - 1; i++) {
		result &= (obs[i].operator_inner(obs[i + 1], "=="));
	}
	return Object(result);
}

/* Return true if obs[0] equal obs[1] equal obs[2] equal .. equal obs[n]*/
Object Primitive::equal(vector<Object>& obs) {
	if (obs.size() != 2) {
		error_handler("ERROR(scheme): eq? and eqaul? take two aurgument");
	}

	bool result = true;
	for (int i = 0; i < obs.size() - 1; i++) {
		result &= (obs[i] == obs[i + 1]);
	}
	return Object(result);
}

/* Return the pair of obs as an Object */
Object Primitive::make_cons(vector<Object>& obs) 
{
	return Object(Cons(obs));
}

/* Return the list of obs as an Object */
Object Primitive::make_list(vector<Object>& obs)
{
	return Object(List(obs));
}

/* Print obs */
Object Primitive::display(vector<Object>& obs)
{
	for (auto &ob : obs) {
		int type = ob.get_type();
#if 0
		cout << "DEBUG display(): type of ob -- "<< ob.get_type_str() << endl;
#endif
		shared_ptr<Cons> spc; /* Used to display Cons */
		switch (type) {
		case INTEGER:
			cout << ob.get_integer() << " ";
			break;
		case REAL:
			cout << ob.get_real() << " ";
			break;
		case BOOLEAN:
			cout << (ob.get_boolean() ? "true" : "false") << " ";
			break;
		case STRING:
			cout << ob.get_string() << " ";
			break;
		case PROCEDURE:
			if (ob.get_proc()->get_type() == PRIMITIVE)
				cout << "<primitive procedure>" << endl; /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
			else if (ob.get_proc()->get_type() == COMPOUND)
				cout << "<compound procedure>" << endl;
			else
				error_handler("ERROR(scheme): unknown procedure -- display");
			break;
		case CONS:
			spc = ob.get_cons();
			cout << "(";
			Primitive::display(vector<Object>{spc->car()});
			cout << ". ";
			Primitive::display(vector<Object>{spc->cdr()});
			/* Backspace, (cons 1 2) print (1 . 2) then, instead of (1 . 2 )*/
			cout << "\b) ";
			break;
		case LIST:
			cout << "(";
			for (auto &ob : ob.get_list()->lst)
				Primitive::display(vector<Object>{ob});
			/* Backspace, (list 1 2 3) print (1 2 3), instead of (1 2 3 ) */
			cout << "\b) ";
			break;
		default: /* UNASSIGNED */
			cerr << "null";
		}
	}
	return Object();
}

/* New line */
Object Primitive::newline(vector<Object>& obs) {
	cout << endl;
	return Object();
}

/* Load code from file and evaluate */
Object Primitive::load(vector<Object>& obs)
{
	if (obs.empty())
		error_handler(string("ERROR(scheme): need a file name -- load\n") +
			"Usage: load \"path/name\" ");
	if (obs[0].get_type() != STRING) {
		error_handler(string("ERROR(scheme): unknown file type/name -- load\n")
			+ "Usage: load \"path/name\" ");
	}

	string filename = obs[0].get_string();
	ifstream ifile(filename.substr(1, filename.size() - 2), ifstream::in);
	if (!ifile) {
		error_handler(string("ERROR(scheme): can't open this file -- ") +
			obs[0].get_string());
	}

	run_evaluator(ifile, 1);
	error_handler("Loading done!");
	return Object();
}