
//Este código se usa en --> https://youtu.be/QujD8Lo7-3Q


#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

LiquidCrystal_I2C lcd(0x27,16,2); 

#define recvPin  11


IRrecv irrecv(recvPin);



void setup() {
  Serial.begin(115200);  
  irrecv.enableIRIn();
   lcd.init();                   
   lcd.backlight();
   lcd.setCursor(1,0);
   lcd.print("> IR Decoder <");
   lcd.setCursor(2,1);
   lcd.print("ElectroTodos");

}

void loop() {
  decode_results  results;        // Somewhere to store the results

  if (irrecv.decode(&results)) {  // Grab an IR code
    dumpInfo(&results);           // Output the results
    dumpRaw(&results);            // Output the results in RAW format
    dumpCode(&results);           // Output the results as source code
    Serial.println("");           // Blank line between entries
    irrecv.resume();              // Prepare for the next value
  }

}



const char *  encoding (decode_results *results)
{
  switch (results->decode_type) {
    default:
    case UNKNOWN:      return "UNKNOWN";       break ;
    case NEC:          return"NEC";           break ;
    case SONY:         return"SONY";          break ;
    case RC5:          return"RC5";           break ;
    case RC6:          return"RC6";           break ;
    case DISH:         return"DISH";          break ;
    case SHARP:        return"SHARP";         break ;
    case JVC:          return"JVC";           break ;
    case SANYO:        return"SANYO";         break ;
    case MITSUBISHI:   return"MITSUBISHI";    break ;
    case SAMSUNG:      return"SAMSUNG";       break ;
    case LG:           return"LG";            break ;
    case WHYNTER:      return"WHYNTER";       break ;
    case AIWA_RC_T501: return"AIWA_RC_T501";  break ;
    case PANASONIC:    return"PANASONIC";     break ;
    case DENON:        return"Denon";         break ;
  }
}

void  dumpInfo (decode_results *results)
{
  // Check if the buffer overflowed
  if (results->overflow) {
    Serial.println("IR code too long. Edit IRremoteInt.h and increase RAWBUF");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("IR code overflow");
    return;
  }

  // Show Encoding standard
  Serial.print("Encoding  : ");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(encoding(results));
  Serial.println(encoding(results));

  // Show Code & length
  Serial.print("Code      : ");
  ircode(results);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
}

void  dumpRaw (decode_results *results)
{
  // Print Raw data
  Serial.print("Timing[");
  Serial.print(results->rawlen-1, DEC);
  Serial.println("]: ");

  for (int i = 1;  i < results->rawlen;  i++) {
    unsigned long  x = results->rawbuf[i] * USECPERTICK;
    if (!(i & 1)) {  // even
      Serial.print("-");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
    } else {  // odd
      Serial.print("     ");
      Serial.print("+");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
      if (i < results->rawlen-1) Serial.print(", "); //',' not needed for last one
    }
    if (!(i % 8))  Serial.println("");
  }
  Serial.println("");                    // Newline
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpCode (decode_results *results)
{
  // Start declaration
  Serial.print("unsigned int  ");          // variable type
  Serial.print("rawData[");                // array name
  Serial.print(results->rawlen - 1, DEC);  // array size
  Serial.print("] = {");                   // Start declaration

  // Dump data
  for (int i = 1;  i < results->rawlen;  i++) {
    Serial.print(results->rawbuf[i] * USECPERTICK, DEC);
    if ( i < results->rawlen-1 ) Serial.print(","); // ',' not needed on last one
    if (!(i & 1))  Serial.print(" ");
  }

  // End declaration
  Serial.print("};");  // 

  // Comment
  Serial.print("  // ");
  encoding(results);
  Serial.print(" ");
  ircode(results);

  // Newline
  Serial.println("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN) {

    // Some protocols have an address
    if (results->decode_type == PANASONIC) {
      Serial.print("unsigned int  addr = 0x");
      Serial.print(results->address, HEX);
      Serial.println(";");
    }

    // All protocols have data
    Serial.print("unsigned int  data = 0x");
    Serial.print(results->value, HEX);
    Serial.println(";");
  }
}

void  ircode (decode_results *results)
{
   lcd.setCursor(0,1);
   
  // Panasonic has an Address
  if (results->decode_type == PANASONIC) {
   
    Serial.print(results->address, HEX);
    lcd.print(results->address, HEX);
    Serial.print(":");
    lcd.print(":");
  }
    else{
      lcd.print("0x");
    }
  // Print Code
  Serial.print(results->value, HEX);
  lcd.print(results->value, HEX);
  
}



