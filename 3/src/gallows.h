/**
 * @file gallows.h
 * @author Johannes Vass <e1327476@student.tuwien.ac.at>
 * @date 19.05.2015
 *
 * @brief This module defines the ten gallow images for the hangman game
 **/

/**
 * @brief an array of 10 strings representing the hangman-gallow before the first and
 * after each possible error. The images show parts of a hanged man, the last image is complete.
 */
static const char const *gallows[] = {
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"    _____________________________\n"
"   |\\ .    .    .    .    .    . \\\n"
"   | \\ .    .    .    .    .    . \\\n"
"   \\  \\ .    .    .    .    .    . \\\n"
"    \\  \\____________________________\\\n"
"     \\ | OSUE SS2015 ascii-hangman  |\n"
"      \\|____________________________|\n",	
	
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"            | |\n"
"            | |\n" 
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"    ________| |__________________\n"
"   |\\ .    .| | .    .    .    . \\\n"
"   | \\ .    | |  .    .    .    . \\\n"
"   \\  \\ .    .    .    .    .    . \\\n"
"    \\  \\____________________________\\\n"
"     \\ | OSUE SS2015 ascii-hangman  |\n"
"      \\|____________________________|\n",	

"             _\n"
"            | \\\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n" 
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"    ________| |__________________\n"
"   |\\ .    .| | .    .    .    . \\\n"
"   | \\ .    | |  .    .    .    . \\\n"
"   \\  \\ .    .    .    .    .    . \\\n"
"    \\  \\____________________________\\\n"
"     \\ | OSUE SS2015 ascii-hangman  |\n"
"      \\|____________________________|\n",	

"             _________________\n"
"            |  _______________|\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n" 
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"    ________| |__________________\n"
"   |\\ .    .| | .    .    .    . \\\n"
"   | \\ .    | |  .    .    .    . \\\n"
"   \\  \\ .    .    .    .    .    . \\\n"
"    \\  \\____________________________\\\n"
"     \\ | OSUE SS2015 ascii-hangman  |\n"
"      \\|____________________________|\n",	

"             _________________\n"
"            |  _______________|\n"
"            | | / /\n"
"            | |/ /\n"
"            | | /\n"
"            | |/\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n" 
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"    ________| |__________________\n"
"   |\\ .    .| | .    .    .    . \\\n"
"   | \\ .    | |  .    .    .    . \\\n"
"   \\  \\ .    .    .    .    .    . \\\n"
"    \\  \\____________________________\\\n"
"     \\ | OSUE SS2015 ascii-hangman  |\n"
"      \\|____________________________|\n",	

"             ___________…_____\n"
"            |  _________))____|\n"
"            | | / /     ||\n"
"            | |/ /      ||\n"
"            | | /       ||\n"
"            | |/        ||\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n" 
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"    ________| |__________________\n"
"   |\\ .    .| | .    .    .    . \\\n"
"   | \\ .    | |  .    .    .    . \\\n"
"   \\  \\ .    .    .    .    .    . \\\n"
"    \\  \\____________________________\\\n"
"     \\ | OSUE SS2015 ascii-hangman  |\n"
"      \\|____________________________|\n",	

"             ___________…_____\n"
"            |  _________))____|\n"
"            | | / /     ||\n"
"            | |/ /      ||\n"
"            | | /       ||.-''.\n"
"            | |/        |/  _  \\\n"
"            | |         ||  `/,|\n"
"            | |         (\\\\`_.'\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"    ________| |__________________\n"
"   |\\ .    .| | .    .    .    . \\\n"
"   | \\ .    | |  .    .    .    . \\\n"
"   \\  \\ .    .    .    .    .    . \\\n"
"    \\  \\____________________________\\\n"
"     \\ | OSUE SS2015 ascii-hangman  |\n"
"      \\|____________________________|\n",
	
"             ___________…_____\n"
"            |  _________))____|\n"
"            | | / /     ||\n"
"            | |/ /      ||\n"
"            | | /       ||.-''.\n"
"            | |/        |/  _  \\\n"
"            | |         ||  `/,|\n"
"            | |         (\\\\`_.'\n"
"            | |        .-`--'.\n"
"            | |       /Y . . Y\\\n"
"            | |      //       \\\n"
"            | |     //         \\\n"
"            | |    ')           (`\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"    ________| |__________________\n"
"   |\\ .    .| | .    .    .    . \\\n"
"   | \\ .    | |  .    .    .    . \\\n"
"   \\  \\ .    .    .    .    .    . \\\n"
"    \\  \\____________________________\\\n"
"     \\ | OSUE SS2015 ascii-hangman  |\n"
"      \\|____________________________|\n",	

"             ___________…_____\n"
"            |  _________))____|\n"
"            | | / /     ||\n"
"            | |/ /      ||\n"
"            | | /       ||.-''.\n"
"            | |/        |/  _  \\\n"
"            | |         ||  `/,|\n"
"            | |         (\\\\`_.'\n"
"            | |        .-`--'.\n"
"            | |       /Y . . Y\\\n"
"            | |      // |   | \\\n"
"            | |     //  | . |  \\\n"
"            | |    ')   |   |   (`\n"
"            | |\n"
"            | |\n"
"            | |\n"
"            | |\n"
"    ________| |__________________\n"
"   |\\ .    .| | .    .    .    . \\\n"
"   | \\ .    | |  .    .    .    . \\\n"
"   \\  \\ .    .    .    .    .    . \\\n"
"    \\  \\____________________________\\\n"
"     \\ | OSUE SS2015 ascii-hangman  |\n"
"      \\|____________________________|\n",
	
"             ___________…_____\n"
"            |  _________))____|\n"
"            | | / /     ||\n"
"            | |/ /      ||\n"
"            | | /       ||.-''.\n"
"            | |/        |/  _  \\\n"
"            | |         ||  `/,|\n"
"            | |         (\\\\`_.'\n"
"            | |        .-`--'.\n"
"            | |       /Y . . Y\\\n"
"            | |      // |   | \\\n"
"            | |     //  | . |  \\\n"
"            | |    ')   |   |   (`\n"
"            | |         ||'||\n"
"            | |         || ||\n"
"            | |         || ||\n"
"            | |         || ||\n"
"    ________| |________/ |_| \\___\n"
"   |\\ .    .| | .    . `-' `-' . \\\n"
"   | \\ .    | |  .    .    .    . \\\n"
"   \\  \\ .    .    .    .    .    . \\\n"
"    \\  \\____________________________\\\n"
"     \\ | OSUE SS2015 ascii-hangman  |\n"
"      \\|____________________________|\n"};
	
