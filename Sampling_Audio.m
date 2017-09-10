
%==========================================================================
%Name:          Audio smaplig
%Author:        Saul Byers
%Class:         ECE 3332 - 301
%Date:          09-09-2017
%Description:  Allows user to change the sampling rate and quantization 
%              levels of an audio signal
%==========================================================================
clear; clc;

BitMax = 16;
BitMin = 1;

%User defined variables
%=========================================================================
fs = input(['Please enter a sampling frequency in Hertz: ']); 
Amplitude = 1;      %Signal amplitude
bits = input(['Please enter how many bits for quantization: ' ]);

while(bits > BitMax || bits < BitMin)
    fprintf(['Error invalid number of bits\n']);
    bits = input(['Please enter how many bits for quantization, must be between ', num2str(BitMin),' and ',num2str(BitMax), ': ' ]);
end
%=========================================================================


%==========================================================================
%Convert audio file to matlab vectors and seperate into left and right
%=========================================================================
[y,fs]=audioread('10 Send for the Man.wma'); % Read sound.wav back into Matlab


n = length(y); %obtain length of sound file
sig_left = zeros(n, 1);
sig_right = zeros(n, 1);

%For loop to take stereo signal and convert it to mono for manipulation
for(i = 1: n)    
    sig_left(i) = y(i,1); 
    sig_right(i) = y(i,2);
end

%Normalize audio signal between -1 and 1
sig_left_norm = sig_left./max(abs(sig_left));     
sig_right_norm = sig_right./max(abs(sig_right)); 

%=========================================================================
%Quantization of audio signal
%=========================================================================

if(bits > 2)
    quant_left = adc_NU(sig_left_norm,Amplitude,bits);
    quant_right = adc_NU(sig_right_norm,Amplitude,bits);
    for(i=1:n)
        quant(i,1) = quant_left(i);
        quant(i,2) = quant_right(i);
    end
elseif(bits > 1)
    %do something
else
    for(i=1:n)
        if (sig_left(i) > 0)
            quant(i,1) = 1;
        else
            quant(i,1) = 0;
        end
        if (sig_right(i) > 0)
            quant(i,2) = 1;
        else
            quant(i,2) = 0;
        end
    end
end



audiowrite('test.wav', quant, fs);

