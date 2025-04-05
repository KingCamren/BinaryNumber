yo. made this project bc i hated doing binary division by hand, and it just ballooned into a complete binary calculator sorta thing for fun

some things to note:
- despite the methods using template types, don't perform operations between a signed and an unsigned binary number. it would give u a wrong answer if u tried on paper or in an assembly language, so i'm not gonna account for that
  - if u're in a situation like if u have a data structure like a vector<BinaryNumber> that might include UnsignedBinaryNumbers, make sure u cast all of the elements to BinaryNumber first; should be easy since BinaryNumber is the parent class
- the to_string() function prints extra zeros so the printed binary number's bit-width is a multiple of 4. this is partly bc i'm learning MIPS rn and we've got word boundaries to stay aligned to
- most of the methods' implementations don't exactly reflect their assembly counterparts. this program is primarily to get outputs from inputs, since the details aren't printed to console anyway

some potential future updates:
- might refactor the project so BinaryNumber is implemented as a vector<bool>
  - currently implemented as a vector<int>, which makes some things more compact (converting between int and char/string), and if a binary number is printed with non-binary values in some bit positions, it's pretty easy to notice
- might implement code so the binary operation methods show their work in the console (especially for methods like operator*() and operator/(), which r pretty messy to do by hand)
  - this might require i rewrite a lot of the methods so they're implemented as they would be in an assembly language
