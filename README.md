# Fingerprint Smart Lock

An Arduino based smart lock that also controls a LED strip.

![img](https://github.com/jaisondasika/fingerprint-smart-lock/blob/main/teaser.gif)

[Full video here](https://www.youtube.com/c/jaisondasika)

This project has been tested on both the [TeensyLC](https://www.pjrc.com/store/teensylc.html) and [SeeduinoXIAO](https://www.seeedstudio.com/Seeeduino-XIAO-Arduino-Microcontroller-SAMD21-Cortex-M0+-p-4426.html) microcontrollers.

## Parts

[SeeeduinoXIAO](https://www.amazon.com/dp/B08CN5YSQF/ref=twister_B0B3D69QX3?_encoding=UTF8&psc=1&_encoding=UTF8&tag=jaisondasika-20&linkCode=ur2&linkId=36caf4bf0586154bc72e1a42ae450bbe&camp=1789&creative=9325)

[Fingerprint Scanner](https://www.amazon.com/%25E3%2580%2590%25F0%259D%2590%258C%25F0%259D%2590%259E%25F0%259D%2590%25AB%25F0%259D%2590%25AB%25F0%259D%2592%259A-%25F0%259D%2590%2582%25F0%259D%2590%25A1%25F0%259D%2590%25AB%25F0%259D%2590%25A2%25F0%259D%2590%25AC%25F0%259D%2590%25AD%25F0%259D%2590%25A6%25F0%259D%2590%259A%25F0%259D%2590%25AC-%25F0%259D%2590%258B%25F0%259D%2590%25A8%25F0%259D%2592%2598%25F0%259D%2590%259E%25F0%259D%2590%25AC%25F0%259D%2590%25AD-%25F0%259D%2590%258F%25F0%259D%2590%25AB%25F0%259D%2590%25A2%25F0%259D%2590%259C%25F0%259D%2590%259E%25E3%2580%2591-Identification/dp/B08HM8QDVW/ref=sr_1_4?keywords=capacitive+fingerprint+scanner&qid=1689473268&sprefix=capacitive+fin%252Caps%252C90&sr=8-4&_encoding=UTF8&tag=jaisondasika-20&linkCode=ur2&linkId=dfe5467376788a657c1543e594eafe95&camp=1789&creative=9325)

[Servo](https://www.amazon.com/Hosyond-Helicopter-Airplane-Control-Compatible/dp/B09V5BR7J5/ref=sr_1_7?crid=JCZWUY2067GK&amp;keywords=metal%252Bgear%252Bservo&amp;qid=1689472302&amp;sprefix=metal%252Bgear%252Bservo%252Caps%252C86&amp;sr=8-7&amp;th=1&_encoding=UTF8&tag=jaisondasika-20&linkCode=ur2&linkId=5c27b55fc82c2038f4f471033087709b&camp=1789&creative=9325)

[LED Strip](https://www.amazon.com/BTF-LIGHTING-Flexible-Individually-Addressable-Non-waterproof/dp/B01CDTEJBG/ref=sr_1_7?crid=IJ92YYOD4G6D&keywords=ws2812b&qid=1689473331&sprefix=ws2812b%252Caps%252C97&sr=8-7&_encoding=UTF8&tag=jaisondasika-20&linkCode=ur2&linkId=504b864929860c815194a5d2b1fda96c&camp=1789&creative=9325)

[Lidar](https://www.amazon.com/Single-Point-Compatible-Rasppbarry-Communication-Interface/dp/B088NVX2L7/ref=sr_1_5?crid=3BWHX0MJSJAFQ&keywords=tf+luna&qid=1689473365&sprefix=tf+luna%252Caps%252C97&sr=8-5&_encoding=UTF8&tag=jaisondasika-20&linkCode=ur2&linkId=65519424085152aacb796fbe436e49ae&camp=1789&creative=9325)

[Button](https://www.amazon.com/weideer-Momentary-Self-Reset-Pre-soldered-R13-507-5-X/dp/B08SQHRRDH/ref=sr_1_6?crid=1YBKIUWVCYYCH&keywords=push+button&qid=1689473513&sprefix=pushbutton%252Caps%252C84&sr=8-6&_encoding=UTF8&tag=jaisondasika-20&linkCode=ur2&linkId=df01b39fd51bfdff86c142f88de1f2f6&camp=1789&creative=9325)

[Hall Effect Sensor](https://www.amazon.com/EPLZON-Effect-Magnetic-Detector-Arduino/dp/B09PG3PGH6/ref=sr_1_5?keywords=hall+effect+sensor&qid=1689473538&sprefix=hall+effec%252Caps%252C90&sr=8-5&_encoding=UTF8&tag=jaisondasika-20&linkCode=ur2&linkId=30fa2cacb817152a0621de3b455e0dd0&camp=1789&creative=9325)

[Buzzer](https://www.amazon.com/Gikfun-Active-Magnetic-Continous-Arduino/dp/B01FVZQ6F6/ref=sr_1_7?crid=20FNFB425N83R&keywords=buzzer&qid=1689473652&s=electronics&sprefix=buzzer%252Celectronics%252C92&sr=1-7&_encoding=UTF8&tag=jaisondasika-20&linkCode=ur2&linkId=cb8f8c632d6e9e5b3e88bc641b620b0e&camp=1789&creative=9325)

[Power Supply](https://www.amazon.com/Henxlco-AC100-240V-Converter-Transformer-Light%25EF%25BC%258CCCTV/dp/B0BFLYDKKD/ref=sr_1_5?keywords=5v+5a+power+supply&qid=1689473427&sr=8-5&_encoding=UTF8&tag=jaisondasika-20&linkCode=ur2&linkId=daecbbd7fe8cc2183ef626ae9937c0fa&camp=1789&creative=9325)
