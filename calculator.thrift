namespace cpp calculator

typedef list<double> Vector

enum BinaryOperation
{
	ADDITION = 1,
	SUBTRACTION = 2,
	MULTIPLICATION = 3,
	DIVISION = 4,
	MODULUS = 5,
}

struct ArithmeticOperation
{
	1:BinaryOperation op,
	2:double lh_term,
	3:double rh_term,
}

exception ArithmeticException
{
	1:string msg,
	2:optional double x,
}

struct Matrix
{
	1:i64 rows,
	2:i64 cols,
	3:list<Vector> data,
}

exception MatrixException
{
	1:string msg,
}

service Calculator
{
	/* Note you can't overload functions */

	double calc (1:ArithmeticOperation op) throws (1:ArithmeticException ae),

	Matrix mult (1:Matrix A, 2:Matrix B) throws (1:MatrixException me),
	Matrix transpose (1:Matrix A) throws (1:MatrixException me),
}