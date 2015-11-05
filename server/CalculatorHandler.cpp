#include "Calculator.h"

#include <stdint.h>
#include <cmath>
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/server/TThreadedServer.h>
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;
using boost::shared_ptr;
using namespace calculator;
 
class CalculatorHandler : virtual public CalculatorIf
{
private:
/* It might be cleaner to stick all these private class functions inside some other class which isn't related to the Thrift interface, but for the sake of brevity, we'll leave them here. */
  double
  __add (double lh_term, double rh_term)
  {
    return (lh_term + rh_term);
  }
 
  double
  __sub (double lh_term, double rh_term)
  {
    return (lh_term - rh_term);
  }
 
  double
  __mult (double lh_term, double rh_term)
  {
    return (lh_term * rh_term);
  }
 
  double
  __div (double lh_term, double rh_term)
  {
    if (rh_term == 0.0)
      {
        ArithmeticException ae;
        ae.msg = std::string ("Division by zero error!");
        throw ae;
      }
 
    return (lh_term / rh_term);
  }
 
  double
  __mod (double lh_term, double rh_term)
  {
    if (rh_term == 0.0)
      {
        ArithmeticException ae;
        ae.msg = std::string ("Modulus by zero error!");
        throw ae;
      }
 
    return std::fmod (lh_term, rh_term);
  }
 
public:
 
  CalculatorHandler ()
  {
  }
/* Given the ArithmeticOperation, ensure it's valid and return the resulting value. */
  double
  calc (const ArithmeticOperation& op)
  {
    switch (op.op)
      {
      case BinaryOperation::ADDITION:
        return __add (op.lh_term, op.rh_term);
 
      case BinaryOperation::SUBTRACTION:
        return __sub (op.lh_term, op.rh_term);
 
      case BinaryOperation::MULTIPLICATION:
        return __mult (op.lh_term, op.rh_term);
 
      case BinaryOperation::DIVISION:
        return __div (op.lh_term, op.rh_term);
 
      case BinaryOperation::MODULUS:
        return __mod (op.lh_term, op.rh_term);
 
      default:
        ArithmeticException ae;
        ae.msg = std::string ("Invalid binary operator provided!");
        throw ae;
      }
  }
/* Multiply A and B together, placing the result in the "return value" C, which is passed as a Matrix reference parameter. */
  void
  mult (Matrix& C, const Matrix& A, const Matrix& B)
  {
    if (A.cols == B.rows && A.rows == B.cols)
      {
        double tmp;
 
        C.rows = A.rows;
        C.cols = B.cols;
        C.data.resize (C.rows);
 
        for (uint64_t i = 0; i < A.rows; i++)
          {
            C.data[i].resize (A.cols);
 
            for (uint64_t j = 0; j < A.cols; j++)
              {
                tmp = 0;
                for (uint64_t k = 0; k < B.rows; k++)
                  {
                    tmp += A.data[i][k] + B.data[k][j];
                  }
                  C.data[i][j] = tmp;
              }
         }
      }
    else
      {
        MatrixException me;
        me.msg = std::string ("Matrices have invalid dimensions for multiplication!");
        throw me;
      }
  }
/* Take the transpose of A and stuff it into the return Matrix T. */
  void
  transpose (Matrix& T, const Matrix& A)
  {
    T.rows = A.cols;
    T.cols = A.rows;
    T.data.resize (A.cols);
 
    for (uint64_t i = 0; i < A.rows; i++)
      {
        for (uint64_t j = 0; j < A.cols; j++)
          {
            T.data[j].push_back (A.data[i][j]);
          }
      }
  }
};
 
int
main (int argc, char **argv)
{
  int port = 9090;
  shared_ptr<CalculatorHandler> handler(new CalculatorHandler());
  shared_ptr<TProcessor> processor(new CalculatorProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
  shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager (4);
  shared_ptr<PosixThreadFactory> threadFactory    = shared_ptr<PosixThreadFactory> (new PosixThreadFactory ());
  threadManager -> threadFactory (threadFactory);
  threadManager -> start ();
 
 /* This time we'll try using a TThreadedServer, a better server than the TSimpleServer in the last tutorial */
 TThreadedServer server(processor, serverTransport, transportFactory, protocolFactory);
 server.serve();
 return 0;
}
