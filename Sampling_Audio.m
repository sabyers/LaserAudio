%==========================================================================
%Name:          Audio Sampling
%Author:        Saul Byers
%Class:         ECE 3332 - 301
%Date:          09-09-2017
%Description:   Allows user to change the sampling rate and quantization 
%               levels of an audio signal
%==========================================================================


%Initialization and constants
%==========================================================================
clear; clc;
BitMax = 16;
BitMin = 1;
Amplitude = 1; 

%User defined variables
%=========================================================================
Song = input(['Please enter full path of song including extension: '], 's');
clc;
SamplingFrequency = input(['Please enter desired sampling frequency in Hertz: ']); 

bits = input(['Please enter how many bits for uniform quantization: ' ]);

while(bits > BitMax || bits < BitMin)
    fprintf(['Error invalid number of bits\n']);
    bits = input(['Please enter how many bits for quantization, must be between ', num2str(BitMin),' and ',num2str(BitMax), ': ' ]);
end

Sound = input(['Please enter playback mode "1" for mono "2" for stereo: ']);

%=========================================================================


%==========================================================================
%Convert audio file to matlab vectors and seperate into left and right
%=========================================================================
clc; fprintf('Importing song\nPlease Wait...');
[OriginalSong,fs]=audioread(Song); % Read sound file into Matlab

%=========================================================================
clc; fprintf('Resampling song\nPlease Wait...');

m=lcm(fs,SamplingFrequency);
    
% determine the up and down sampling rates
up=m/fs;
down=m/SamplingFrequency;
    
% resample the input using the computed up/down rates
ResampledSong=resample(OriginalSong,up,down);
    
%=========================================================================



n = length(ResampledSong); %obtain length of sound file
sig_left = zeros(n, 1);
sig_right = zeros(n, 1);

%For loop to take stereo signal and convert it to mono for manipulation
for(j = 1: n)    
    sig_left(j) = ResampledSong(j,1); 
    sig_right(j) = ResampledSong(j,2);
end

%Normalize audio signal between -1 and 1
sig_left_norm = sig_left./max(abs(sig_left));     
sig_right_norm = sig_right./max(abs(sig_right)); 
tsig_left_norm = transpose(sig_left_norm);
tsig_right_norm = transpose(sig_right_norm);

%=========================================================================
%Quantization of audio signal
%=========================================================================
clc; fprintf('Quantizing song\nPlease Wait...');
if (Sound == 1)               %Mono quantization
    quant = zeros(n,1);
    if(bits > 1)
    quant_right = adc_NU(tsig_right_norm,Amplitude,bits);
    for(j=1:n)
        quant(j) = quant_right(1, j);
    end
    else
        for(j=1:n)
            if (sig_right(j) > 0)
                quant(j) = 1;
            else
                quant(j) = 0;
            end
        end
    end
else                        %Stereo quantization
    quant = zeros(n,2);
    if(bits > 1)
    quant_left = adc_NU(tsig_left_norm,Amplitude,bits);
    quant_right = adc_NU(tsig_right_norm,Amplitude,bits);
    for(j=1:n)
        quant(j,1) = quant_left(1,j);
        quant(j,2) = quant_right(1,j);
    end
    else
        for(j=1:n)
           
            if (sig_left(j) > 0)
                quant(j,1) = 1;
            else
                quant(j,1) = 0;
            end
            if (sig_right(j) > 0)
                quant(j,2) = 1;
            else
                quant(j,2) = 0;
            end
        end
    end
end

%=========================================================================
%Running Signal through LPF
%=========================================================================

fprintf('\n');
Fltr = input(['Do you want to filter the siganl? (Y/N): '], 's');

if (Fltr == 'Y' || Fltr == 'y')
clc; fprintf('Filtering song\nPlease Wait...');
quant = quant.*10;
fc1 = 20; % Cut off frequency
[b,a] = cheby2(6,3, 2*pi/fc1); % Butterworth filter
quant = filter(b,a,quant); % Will be the filtered signal
quant = quant + 1;
quant = quant./max(quant(:));
end


fprintf('\n\nDone Converting\n');

%=========================================================================
%Write audio file
%=========================================================================
Title = input(['Please enter title of song sample: '], 's');
clc; fprintf('Exporting song\n');
SongTitle=[Title,'.wav'];
audiowrite(SongTitle, quant, SamplingFrequency);
fprintf('Your song is Ready!\n');

%=========================================================================
%End of all code
%=========================================================================
