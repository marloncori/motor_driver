#define PROGMEM ICACHE_RODATA_ATTR
#define ICACHE_RODATA_ATTR  __attribute__((section(".irom.text")))

//Which places the variable in the .irom.text section in flash. Placing strings in flash requires using any of the methods above.
//PROGMEM is needed, because strings would be otherwise saved in the RAM, but we want them to be stored in the Flash memory instead

static const char xyz[] PROGMEM = "This is a string stored in flash";

#define PGM_P       const char *
#define PGM_VOID_P  const void *
#define PSTR(s) (__extension__({static const char __c[] PROGMEM = (s); &__c[0];}))

void myFuction(void){
    PGM_P xyz = PSTR("Store this string in flash memory");
    const char *abc = PSTR("Also store this one in flash.");
}

//Functions to read back from PROGMEM
int memcmp_P(const void* buf1, PGM_VOID_P buf2P, size_t size);
void* memccpy_P(void* dest, PGM_VOID_P src, int c, size_t count);
void* memmem_P(const void* buf, size_t bufSize, PGM_VOID_P findP, size_t findPSize);
void* memcpy_P(void* dest, PGM_VOID_P src, size_t count);
char* strncpy_P(char* dest, PGM_P src, size_t size);
char* strcpy_P(dest, src)
char* strncat_P(char* dest, PGM_P src, size_t size);
char* strcat_P(dest, src)
int strncmp_P(const char* str1, PGM_P str2P, size_t size);
int strcmp_P(str1, str2P)
int strncasecmp_P(const char* str1, PGM_P str2P, size_t size);
int strcasecmp_P(str1, str2P)
size_t strnlen_P(PGM_P s, size_t size);
size_t strlen_P(strP)
char* strstr_P(const char* haystack, PGM_P needle);
int printf_P(PGM_P formatP, ...);
int sprintf_P(char *str, PGM_P formatP, ...);
int snprintf_P(char *str, size_t strSize, PGM_P formatP, ...);
int vsnprintf_P(char *str, size_t strSize, PGM_P formatP, va_list ap);

#define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#define F(string_literal) (FPSTR(PSTR(string_literal)))

//FPSTR() allows you to define a global flash string and then use it in any function that takes __FlashStringHelper.
//SO sharing common strings is possible using FPSTR() .
static const char xyz[] PROGMEM = "This is a string stored in ESP8266 Flash.";
Serial.println(FPSTR(xyz));


//F() allows you to define these flash strings in place, but you can’t use them anywhere else.
Serial.println(F("This is a string stored in flash"));

String(const char *cstr = nullptr); // constructor from const char *
String(const String &str); // copy constructor
String(const __FlashStringHelper *str); // constructor for flash strings

//This allows me to write:
String mystring(F("This string is stored in flash"));

//How do I write a function to use __FlashStringHelper? Simples:
// cast the pointer back to a PGM_P and use the _P functions shown above.

unsigned char String::concat(const __FlashStringHelper * str) {
    if (!str) return 0; // return if the pointer is void
    int length = strlen_P((PGM_P)str); // cast it to PGM_P, which is basically const char *, and measure it using the _P version of strlen.
    if (length == 0) return 1;
    unsigned int newlen = len + length;
    if (!reserve(newlen)) return 0; // create a buffer of the correct length
    strcpy_P(buffer + len, (PGM_P)str); //copy the string in using strcpy_P
    len = newlen;
    return 1;

//Now declare a global flash string and use it anywhere in the code.
static const char xyz[] PROGMEM = "This is a string stored in flash. Len = %u";

void setup() {
    Serial.begin(115200); Serial.println();
    Serial.println( FPSTR(xyz) ); // just prints the string, must convert it to FlashStringHelper first using FPSTR().
    Serial.printf_P( xyz, strlen_P(xyz)); // use printf with PROGMEM string


//Declare inline flash string
void setup() {
    Serial.begin(115200); Serial.println();
    Serial.println( F("This is an inline string")); //
    Serial.printf_P( PSTR("This is an inline string using printf %s"), "hello");
}

//Declare and use data in PROGMEM
onst size_t len_xyz = 30;
const uint8_t xyz[] PROGMEM = {
  0x53, 0x61, 0x79, 0x20, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20,
  0x74, 0x6f, 0x20, 0x4d, 0x79, 0x20, 0x4c, 0x69, 0x74, 0x74,
  0x6c, 0x65, 0x20, 0x46, 0x72, 0x69, 0x65, 0x6e, 0x64, 0x00};

 void setup() {
     Serial.begin(115200); Serial.println();
     uint8_t * buf = new uint8_t[len_xyz];
     if (buf) {
      memcpy_P(buf, xyz, len_xyz);
      Serial.write(buf, len_xyz); // output the buffer.
     }
 }

//declare some data in PROGMEM and retrieve one byte from it.
const size_t len_xyz = 30;
const uint8_t xyz[] PROGMEM = {
  0x53, 0x61, 0x79, 0x20, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20,
  0x74, 0x6f, 0x20, 0x4d, 0x79, 0x20, 0x4c, 0x69, 0x74, 0x74,
  0x6c, 0x65, 0x20, 0x46, 0x72, 0x69, 0x65, 0x6e, 0x64, 0x00
};

void setup() {
  Serial.begin(115200); Serial.println();
  for (int i = 0; i < len_xyz; i++) {
    uint8_t byteval = pgm_read_byte(xyz + i);
    Serial.write(byteval); // output the buffer.
  }
}

//Declare an Array of string and retrieve an element from it, for texts to be printed on LCD, for example.
// Define Strings
const char string_0[] PROGMEM = "String 0";
const char string_1[] PROGMEM = "String 1";
const char string_2[] PROGMEM = "String 2";
const char string_3[] PROGMEM = "String 3";
const char string_4[] PROGMEM = "String 4";
const char string_5[] PROGMEM = "String 5";

// Initialize Table of Strings
const char* const string_table[] PROGMEM = { string_0, string_1, string_2, string_3, string_4, string_5 };

char buffer[30]; // buffer for reading the string to (needs to be large enough to take the longest string

void setup() {
  Serial.begin(9600);
  Serial.println("OK");
}

void loop() {
  for (int i = 0; i < 6; i++) {
    strcpy_P(buffer, (char*)pgm_read_dword(&(string_table[i])));
    Serial.println(buffer);
    delay(500);
  }
}

