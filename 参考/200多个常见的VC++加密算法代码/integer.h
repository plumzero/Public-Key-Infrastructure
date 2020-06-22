//Download by http://www.NewXing.com
#ifndef CRYPTOPP_INTEGER_H
#define CRYPTOPP_INTEGER_H

#include "cryptlib.h"
#include "misc.h"

#include <iosfwd>

NAMESPACE_BEGIN(CryptoPP)

/// multiple precision integer and basic arithmetics
/** This class can represent positive and negative integers
	with absolute value less than 2**(8 * sizeof(word) * 2**sizeof(int)).
*/
class Integer
{
public:
	//@Man: ENUMS, EXCEPTIONS, and TYPEDEFS
	//@{
		/// division by zero exception
		class DivideByZero : public Exception 
		{
		public: 
			DivideByZero() : Exception("Integer: division by zero") {}
		};

		///
		class RandomNumberNotFound : public Exception 
		{
		public: 
			RandomNumberNotFound() : Exception("Integer: random number not found") {}
		};

		///
		enum Signedness {
		///
			UNSIGNED, 
		///
			SIGNED};

		///
		enum RandomNumberType {
		///
			ANY, 
		///
			PRIME};
	//@}

	//@Man: CREATORS
	//@{
		/// creates the zero integer
		Integer();

		/// copy constructor
		Integer(const Integer& t);

		/// convert from signed long
		Integer(signed long value);

		/// convert from string
		/** str can be in base 2, 8, 10, or 16.  Base is determined by a 
			case insensitive suffix of 'h', 'o', or 'b'.  No suffix means base 10.
		*/
		Integer(const char *str);

		/// convert from big-endian byte array
		Integer(const byte *encodedInteger, unsigned int byteCount, Signedness s=UNSIGNED);

		/// convert from Basic Encoding Rules encoded byte array
		Integer(const byte *BEREncodedInteger);

		/// convert from BER encoded byte array stored in a BufferedTransformation object
		Integer(BufferedTransformation &bt);

		/// create a random integer
		/** The random integer created is uniformly distributed over [0, 2**bitcount). */
		Integer(RandomNumberGenerator &rng, unsigned int bitcount);

		/// create a random integer of special type
		/** Ideally, the random integer created should be uniformly distributed
			over {x | min <= x <= max and x is of rnType and x % mod == equiv}.
			However the actual distribution may not uniform because sequential
			search is used to find an appropriate number from a random starting
			point.
			May return (with very small probability) a pseudoprime when a prime
			is requested and max > lastSmallPrime*lastSmallPrime.
			Throws RandomNumberNotFound if the set is empty.
		*/
		Integer(RandomNumberGenerator &rng, const Integer &min, const Integer &max, RandomNumberType rnType=ANY, const Integer &equiv=Zero(), const Integer &mod=One());

		/// create the integer 2**e
		static Integer Power2(unsigned int e);

		/// avoid calling constructors for these frequently used integers
		static const Integer &Zero();
		///
		static const Integer &One();
	//@}

	//@Man: ACCESSORS
	//@{
		/// return equivalent signed long if possible, otherwise undefined
		signed long ConvertToLong() const;

		/// minimum number of bytes to encode this integer
		/** MinEncodedSize of 0 is 1 */
		unsigned int MinEncodedSize(Signedness=UNSIGNED) const;
		/// encode in big-endian format
		/** unsigned means ignore sign, signed means use two's complement.
			if outputLen < MinEncodedSize, the most significant bytes will be dropped
			if outputLen > MinEncodedSize, the most significant bytes will be padded
		*/
		unsigned int Encode(byte *output, unsigned int outputLen, Signedness=UNSIGNED) const;

		/// encode integer using Distinguished Encoding Rules, returns size of output
		unsigned int DEREncode(byte *output) const;
		/// encode using DER, put result into a BufferedTransformation object
		unsigned int DEREncode(BufferedTransformation &bt) const;

		/// number of significant bits = floor(log2(abs(*this))) + 1
		unsigned int BitCount() const;
		/// number of significant bytes = ceiling(BitCount()/8)
		unsigned int ByteCount() const;
		/// number of significant words = ceiling(ByteCount()/sizeof(word))
		unsigned int WordCount() const;

		/// return the n-th bit, n=0 being the least significant bit
		bool GetBit(unsigned int n) const;
		/// return the n-th byte
		byte GetByte(unsigned int n) const;

		///
		bool IsNegative() const	{return sign == NEGATIVE;}
		///
		bool NotNegative() const {return sign == POSITIVE;}
		///
		bool IsPositive() const	{return sign == POSITIVE && !!*this;}
		///
		bool IsEven() const	{return GetBit(0) == 0;}
		///
		bool IsOdd() const	{return GetBit(0) == 1;}
	//@}

	//@Man: MANIPULATORS
	//@{
		///
		Integer&  operator=(const Integer& t);

		///
		Integer&  operator+=(const Integer& t);
		///
		Integer&  operator-=(const Integer& t);
		///
		Integer&  operator*=(const Integer& t)  {return *this = *this*t;}
		///
		Integer&  operator/=(const Integer& t)  {return *this = *this/t;}
		///
		Integer&  operator%=(const Integer& t)  {return *this = *this%t;}
		///
		Integer&  operator/=(word t)  {return *this = *this/t;}
		///
		Integer&  operator%=(word t)  {return *this = *this%t;}

		///
		Integer&  operator<<=(unsigned int);
		///
		Integer&  operator>>=(unsigned int);

		///
		void Decode(const byte *input, unsigned int inputLen, Signedness=UNSIGNED);

		///
		void BERDecode(const byte *input);
		///
		void BERDecode(BufferedTransformation &bt);

		///
		void Randomize(RandomNumberGenerator &rng, unsigned int bitcount);
		///
		void Randomize(RandomNumberGenerator &rng, const Integer &min, const Integer &max);
		/// set this Integer to a random element of {x | min <= x <= max and x is of rnType and x % mod == equiv}
		/** returns false if the set is empty */
		bool Randomize(RandomNumberGenerator &rng, const Integer &min, const Integer &max, RandomNumberType rnType, const Integer &equiv=Zero(), const Integer &mod=One());

		/// set the n-th bit to value
		void SetBit(unsigned int n, bool value=1);
		/// set the n-th byte to value
		void SetByte(unsigned int n, byte value);

		///
		void Negate();
		///
		void SetPositive() {sign = POSITIVE;}
		///
		void SetNegative() {if (!!(*this)) sign = NEGATIVE;}

		///
		void swap(Integer &a);
	//@}

	//@Man: UNARY OPERATORS
	//@{
		///
		bool		operator!() const;
		///
		Integer		operator+() const {return *this;}
		///
		Integer		operator-() const;
		///
		Integer&	operator++();
		///
		Integer&	operator--();
		///
		Integer		operator++(int) {Integer temp = *this; ++*this; return temp;}
		///
		Integer		operator--(int) {Integer temp = *this; --*this; return temp;}
	//@}

	//@Man: BINARY OPERATORS
	//@{
		///
		friend Integer operator+(const Integer &a, const Integer &b);
		///
		friend Integer operator-(const Integer &a, const Integer &b);
		///
		friend Integer operator*(const Integer &a, const Integer &b);
		///
		friend Integer operator/(const Integer &a, const Integer &b);
		///
		friend Integer operator%(const Integer &a, const Integer &b);
		///
		friend Integer operator/(const Integer &a, word b);
		///
		friend word    operator%(const Integer &a, word b);

		///
		Integer operator>>(unsigned int n) const	{return Integer(*this)>>=n;}
		///
		Integer operator<<(unsigned int n) const	{return Integer(*this)<<=n;}

		///
		friend bool operator==(const Integer& a, const Integer& b) {return (a.Compare(b)==0);}
		///
		friend bool operator!=(const Integer& a, const Integer& b) {return (a.Compare(b)!=0);}
		///
		friend bool operator> (const Integer& a, const Integer& b) {return (a.Compare(b)> 0);}
		///
		friend bool operator>=(const Integer& a, const Integer& b) {return (a.Compare(b)>=0);}
		///
		friend bool operator< (const Integer& a, const Integer& b) {return (a.Compare(b)< 0);}
		///
		friend bool operator<=(const Integer& a, const Integer& b) {return (a.Compare(b)<=0);}
	//@}

	//@Man: OTHER ARITHMETIC FUNCTIONS
	//@{
		/// signed comparison
		/** returns:
			\begin{itemize} 
			\item -1 if *this < a
			\item  0 if *this = a
			\item  1 if *this > a
			\end{itemize}
		*/
		int Compare(const Integer& a) const;

		///
		Integer AbsoluteValue() const;
		///
		Integer Doubled() const {return *this + *this;}
		///
		Integer Squared() const {return *this * (*this);}
		/// extract square root, if negative return 0, else return floor of square root
		Integer SquareRoot() const;
		/// return whether this integer is a perfect square
		bool IsSquare() const;

		/// is 1 or -1
		bool IsUnit() const;
		/// return inverse if 1 or -1, otherwise return 0
		Integer MultiplicativeInverse() const;

		/// modular multiplication
		friend Integer a_times_b_mod_c(const Integer &x, const Integer& y, const Integer& m);
		/// modular exponentiation
		friend Integer a_exp_b_mod_c(const Integer &x, const Integer& e, const Integer& m);

		/// calculate r and q such that (a == d*q + r) && (0 <= r < abs(d))
		static void Divide(Integer &r, Integer &q, const Integer &a, const Integer &d);
		/// use a faster division algorithm when divisor is short
		static word ShortDivide(Integer &q, const Integer &a, word d);

		/// greatest common divisor
		static Integer Gcd(const Integer &a, const Integer &n);
		/// calculate multiplicative inverse of *this mod n
		Integer InverseMod(const Integer &n) const;
		///
		word InverseMod(word n) const;
	//@}

	//@Man: INPUT/OUTPUT
	//@{
		///
		friend std::istream& operator>>(std::istream& in, Integer &a);
		///
		friend std::ostream& operator<<(std::ostream& out, const Integer &a);
	//@}

private:
	friend class ModularArithmetic;
	friend class MontgomeryRepresentation;
	friend class HalfMontgomeryRepresentation;

	Integer(word value, unsigned int length);

	int PositiveCompare(const Integer &t) const;
	friend void PositiveAdd(Integer &sum, const Integer &a, const Integer &b);
	friend void PositiveSubtract(Integer &diff, const Integer &a, const Integer &b);
	friend void PositiveMultiply(Integer &product, const Integer &a, const Integer &b);
	friend void PositiveDivide(Integer &remainder, Integer &quotient, const Integer &dividend, const Integer &divisor);

	enum Sign {POSITIVE=0, NEGATIVE=1};

	SecWordBlock reg;
	Sign sign;
};

NAMESPACE_END

NAMESPACE_BEGIN(std)
inline void swap(CryptoPP::Integer &a, CryptoPP::Integer &b)
{
	a.swap(b);
}
NAMESPACE_END

#endif
