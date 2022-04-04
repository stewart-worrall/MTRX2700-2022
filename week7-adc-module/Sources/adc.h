#ifndef ADC_HEADER
#define ADC_HEADER


// define a function pointer to handle a callback
//   from an adc measurement
typedef void (*ADCHandler)(unsigned char);

// define an ADC interface as the channel, and the function to call after making a measurement
typedef struct ADCInterface {
  ADCHandler callback_function;
  unsigned char channel_to_read;
} ADCInterface;


// registers the ADC channel to a callback
void RegisterADC(int channel_to_read, ADCHandler callback_function);


// timer 6 is used to trigger the ADC reads
__interrupt void TC6_ISR(void);

#endif
