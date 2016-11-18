/*
StructInitialization
(c) 2016
original authors: David I. Schwartz
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*	This tutorial illustrates the concepts of POD and struct 
	initialization.
*	It also covers issues such as aggregates, Brace-Initialization, 
	predefined MSDN macro, and member access operators.
*/

//
// questions: can you give default values to structs?
//            how can you access struct values and functions?
//
// structs are PODs (plain old data type):
//   - essentially, classes with everything public, no inheritance, (and only trivial special member functions for C++11)
//   - http://en.wikipedia.org/wiki/Plain_old_data_structure (C++03 says no special member functions)
//   - C++03: http://stackoverflow.com/questions/5442717/cant-c-pod-type-have-any-constructor
//            http://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special/4178176#4178176
//   - C++11: http://en.wikipedia.org/wiki/C%2B%2B11#Modification_to_the_definition_of_plain_old_data
// 
// see default-initialize standard for C++03: http://stackoverflow.com/questions/1613341/what-do-the-following-phrases-mean-in-c-zero-default-and-value-initializat

// C++11 initialization (includes C++03, so it's worth reading):
//   - summary: http://www.informit.com/articles/article.aspx?p=1852519
//   - references: http://en.cppreference.com/w/cpp/language/value_initialization and http://en.cppreference.com/w/cpp/language/default_initialization

// C++03:
//   - summary: http://stackoverflow.com/questions/1613341/what-do-the-following-phrases-mean-in-c-zero-default-and-value-initializat
//   - examples & explanations: http://stackoverflow.com/questions/5914422/proper-way-to-initialize-c-structs
//                              http://stackoverflow.com/questions/2722879/calling-constructors-in-c-without-new/2723266#2723266
//                              http://stackoverflow.com/questions/8280023/does-a-c-struct-have-a-default-constructor

#include <iostream>
using namespace std;

// demoing a POD
// could also say class Thing if you make everything public
// adapted from the above StackOverflow articles
struct Thing {
	int x;      // not initialized as zero! (see examples below)
	int y = 10; // allowed in VS2013
	void test() const { cout << __FUNCTION__ << endl; } // predefined MSDN macro: http://msdn.microsoft.com/en-us/library/b0084kay%28v=vs.100%29.aspx

	// C++03: no special member functions allowed to keep POD "status" (http://en.wikipedia.org/wiki/Plain_old_data_structure):
	// If you want to test the following, ensure that you uncomment the 'public:' block:
	 //public:
	 //Thing() {}                                                                          // constructor
	 //Thing(int a) : x(a) {}                                                              // constructor
	 //Thing& operator=(const Thing& other) { cout << "test op=" << endl; return *this; }  // operator overloading
	 //Thing (const Thing& other) { cout << "test cpy-ctor" << endl; }                     // use-defined constructor
	 //~Thing() { cout << "I have been deallocated!" << endl; }                            // user-defined destructor
};

int main() {
	                                                                 //                               | POD                                                           | not POD   
	cout << "\nTest stack allocation and initialization: " << endl;  //                               | output: Test stack allocation and initialization:             | output: Test stack allocation and initialization:
	Thing t1;                 // Members are NOT initialized
	Thing t2 = Thing();       // Members are zero-initialized-->for PODs! (see above links...slow down...will take awhile)
	t1.test();                // t1 is indeed allocated on the stack                                  | output: Thing::test                                           | output: Thing::test
	cout << t1.x << endl;     // x has "garbage" value (press Ignore if prompted)                     | output: -858993460                                            | output: -858993460
	cout << t2.x << endl;     // why does x have garbage value? if you didn't include "int y=10",     | output: 0                                                     | output: 
							  // you have a POD -- as if constructor (with initializer) gets called   
	t2.test();                // runs the function                                                    | output: Thing::test
	
	cout << "\nTest alternative POD initialization using {}: " << endl;                            // | output: Test alternative POD initialization using {}:
	cout << "\nNOTE: uncomment the 'int y = 10' in the struct to test more code herein." << endl;  // | output: NOTE: uncomment the 'int y = 10' in the struct to test more code herein.
	Thing t3 = {};            // initialize fields of t3 to defaults
	cout << t3.x << endl;     //                                                                      | output: 0
	// Thing t4 = {1, 2};     // can you treat this struct as an aggregate? Ans: No, because Thing has user-declared constructors.
	                          // see aggregates: http://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special)
	// cout << t4.x << endl;   
	Thing t5{};               // C++11 only (brace-initialization)
	Thing t6 = Thing{};       // C++11 only (brace-initialization)
	cout << t5.y << endl;   // uncomment the "int y=10" in the struct                              // | output: 10                                                    
	
	cout << "\nTest copying: " << endl;                                                            // | output: Test copying:                                                         
	Thing t7 = t2;            // makes a copy of t3-->not an alias! ("value" objects)                                                                                 | output: test cpy-ctor
	cout << &t2 << endl;      // proof: t2 and t3 are different variables                          // | output: 0045F8A4                                              | output: 00CBFA9C
	cout << &t7 << endl;      //        rem that they're both on the stack                         // | output: 0045F864                                              | output: 00CBFA5C
	t7.x = 10;                // change t3's x
	cout << t2.x << endl;     // note that t2.x is still 0 (only if you remove the "int y=10")     // | output: 0                                                     | output: -858993460
	
	cout << "\nTest heap allocation: " << endl;                                                    // | output: Test heap allocation:
	Thing* t8 = new Thing;    // Members are NOT initialized                                       
	Thing* t9 = new Thing();  // Members are zero-initialized                                       
	t8->test();               // runs test                                                         // | output: Thing::test
	t9->test();               // runs test                                                         // | output: Thing::test
	cout << t8->x << endl;    // causes "complaint" because of x not having a "garbage" value      // | output: -842150451                                            | output: -842150451
	cout << t9->x << endl;                                                                         // | output: 0                                                     | output: -842150451
	delete t8;
	delete t9;
	                                                                                               // deallocate the memory for t1-t9                                 | output: I have been deallocated!
																								   //                                                                 | output: I have been deallocated!
																								   //                                                                 | output: I have been deallocated!
																								   //                                                                 | output: I have been deallocated!
																								   //                                                                 | output: I have been deallocated!
																								   //                                                                 | output: I have been deallocated!
																								   //                                                                 | output: I have been deallocated!
																								   //                                                                 | output: I have been deallocated!


	
	cout << "\nTest 'crazy' (but old-school) way to set memory (generally, avoid): " << endl;                                                                     // | output: Test 'crazy' (but old-school) way to set memory (generally, avoid):
	// this example will generate a warning--it's "abusing" how C++ deals with non-initialized fields
	// memset and PODs:
	// http://stackoverflow.com/questions/2204176/how-to-initialise-memory-with-new-operator-in-c
	// http://stackoverflow.com/questions/1975916/should-c-programmer-avoid-memset (see also)
	// http://msdn.microsoft.com/en-us/library/1fdeehz6%28v=vs.100%29.aspx
	// http://stackoverflow.com/questions/1998752/which-one-to-use-memset-or-value-initialization-to-zero-out-a-struct
	// see also std::fill: http://msdn.microsoft.com/en-us/library/kaxe2wy9%28v=vs.100%29.aspx
	Thing t10;                    // start with a POD with no default values
	cout << t10.x << endl;        // confirm that x is garbage value                                                                                                 | output: -858993460
	memset(&t10, 0, sizeof(Thing)); // old school! structs resemble arrays-->starting at t10's location, start filling "it" (the "array") with default
	cout << t10.x << endl;        // check--0!                                                                                                                       | output: 0
	
}



