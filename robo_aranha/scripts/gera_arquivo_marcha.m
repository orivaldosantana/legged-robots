%  Padroes essenciais para a locomocao no modo wave(lento) 
% L1 A 	1.0  0.5  0.0  0.1  0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9
% L1 B 	0.0  1.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0
% L2 A	0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9  1.0  0.5  0.0  0.1 
% L2 B	0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  1.0  0.0  0.0   
% L3 A	0.6  0.7  0.8  0.9  1.0  0.5  0.0  0.1  0.2  0.3  0.4  0.5
% L3 B	0.0  0.0  0.0  0.0  0.0  1.0  0.0  0.0  0.0  0.0  0.0  0.0 
% R1 A	0.4  0.5  0.6  0.7  0.8  0.9  1.0  0.5  0.0  0.1  0.2  0.3
% R1 B 	0.0  0.0  0.0  0.0  0.0  0.0  0.0  1.0  0.0  0.0  0.0  0.0
% R2 A	0.8  0.9  1.0  0.5  0.0  0.1  0.2  0.3  0.4  0.5  0.6  0.7
% R2 B	0.0  0.0  0.0  1.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0
% R3 A	0.0  0.1  0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9  1.0  0.5
% R3 B	0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  1.0

%  Padroes essenciais para a locomocao no modo tripoid (rapido) 
% L1 A 	0.0  0.5  1.0  0.5  0.0
% L1 B 	0.0  1.0  0.0  1.0  0.0
% L2 A	1.0  0.5  0.0  0.5  1.0
% L2 B	1.0  0.0  1.0  0.0  1.0
% L3 A	0.0  0.5  1.0  0.5  0.0
% L3 B	0.0  1.0  0.0  1.0  0.0
% R1 A	1.0  0.5  0.0  0.5  1.0
% R1 B 	1.0  0.0  1.0  0.0  1.0
% R2 A	0.0  0.5  1.0  0.5  0.0
% R2 B	0.0  1.0  0.0  1.0  0.0
% R3 A	1.0  0.5  0.0  0.5  1.0
% R3 B	1.0  0.0  1.0  0.0  1.0

fast_patterns = zeros(5,12);
fast_patterns(:,1) = [1.0  0.5  0.0  0.5  1.0];
fast_patterns(:,2) = [0.0  1.0  0.0  0.0  0.0];
fast_patterns(:,3) = [0.0  0.5  1.0  0.5  0.0];
fast_patterns(:,4) = [0.0  0.0  0.0  1.0  0.0];
fast_patterns(:,5) = [1.0  0.5  0.0  0.5  1.0];
fast_patterns(:,6) = [0.0  1.0  0.0  0.0  0.0];
fast_patterns(:,7) = [0.0  0.5  1.0  0.5  0.0];
fast_patterns(:,8) = [0.0  0.0  0.0  1.0  0.0];
fast_patterns(:,9) = [1.0  0.5  0.0  0.5  1.0];
fast_patterns(:,10) = [0.0  1.0  0.0  0.0  0.0];
fast_patterns(:,11) = [0.0  0.5  1.0  0.5  0.0];
fast_patterns(:,12) = [0.0  0.0  0.0  1.0  0.0];

basic_patterns = zeros(13,12);
basic_patterns(:,1) = [1.0  0.5  0.0  0.1  0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9 1.0];
basic_patterns(:,2) = [0.0  1.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0 0.0];
basic_patterns(:,3) = [0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9  1.0  0.5  0.0  0.1 0.2];
basic_patterns(:,4) = [0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  1.0  0.0  0.0 0.0];
basic_patterns(:,5) = [0.6  0.7  0.8  0.9  1.0  0.5  0.0  0.1  0.2  0.3  0.4  0.5 0.6];
basic_patterns(:,6) = [0.0  0.0  0.0  0.0  0.0  1.0  0.0  0.0  0.0  0.0  0.0  0.0 0.0];
basic_patterns(:,7) = [0.4  0.5  0.6  0.7  0.8  0.9  1.0  0.5  0.0  0.1  0.2  0.3 0.4];
basic_patterns(:,8) = [0.0  0.0  0.0  0.0  0.0  0.0  0.0  1.0  0.0  0.0  0.0  0.0 0.0];
basic_patterns(:,9) = [0.8  0.9  1.0  0.5  0.0  0.1  0.2  0.3  0.4  0.5  0.6  0.7 0.8];
basic_patterns(:,10) = [0.0  0.0  0.0  1.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0 0.0];
basic_patterns(:,11) = [0.0  0.1  0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9  1.0  0.5 0.0];
basic_patterns(:,12) = [0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  0.0  1.0 0.0];

basic_patterns_left = fast_patterns;
basic_patterns_right = fast_patterns;
%basic_patterns_left = basic_patterns;
%basic_patterns_right = basic_patterns; 

basic_patterns_left(:,1)  = 1 - basic_patterns_left(:,1);
basic_patterns_left(:,3)  = 1 - basic_patterns_left(:,3);
basic_patterns_left(:,5)  = 1 - basic_patterns_left(:,5);

basic_patterns_right(:,7)  = 1 - basic_patterns_right(:,7);
basic_patterns_right(:,9)  = 1 - basic_patterns_right(:,9);
basic_patterns_right(:,11)  = 1 - basic_patterns_right(:,11);


%prepare_states(basic_patterns_right,4);
%prepare_states(basic_patterns_left,4);
prepare_states(basic_patterns,4);
%prepare_states(basic_patterns_left,4);
%prepare_states(basic_patterns_right,4);
%prepare_states(fast_patterns,4);
