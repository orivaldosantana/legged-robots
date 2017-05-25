function deg = para_angulo(rad)
	deg=(180*rad)/pi
end


file = '../simulador/simulator_trajectory_gait_wave.dat';

pwd

load ("-text", file, "output_cnn_hexa");

k = 1:length(output_cnn_hexa(:,2));
%p1 = plot(k,output_cnn_hexa(:,4));
normalizado = zeros(41,12);
angulos = zeros(41,12);
angulosCorrecao = zeros(41,12);
 for coluna = 1:12
    	%a =  para_angulo(output_cnn_hexa(:,coluna));
    	a = output_cnn_hexa(:,coluna);
	valorMin = min(a);
	valorMax = max(a);
	normalizado(:,coluna) = (a - valorMin)/(valorMax - valorMin);
	if ( mod(coluna,2) == 0 )
		angulos(:,coluna) = -30 + normalizado(:,coluna)*60;
	else
		angulos(:,coluna) = - normalizado(:,coluna)*30;
	end
	
end

% correção das posições 

% L 1: 1 e 2 -> 1 e 2
% R 2: 3 e 4 -> 9 e 10
% L 3: 5 e 6 -> 5 e 6
% R 1: 7 e 8 -> 11 e 12
% L 2: 9 e 10 -> 3 e 4
% R 3: 11 e 12 -> 8 e 9

 angulosCorrecao(:,1) = angulos(:,1);
 angulosCorrecao(:,2) = angulos(:,2);
angulosCorrecao(:,3) = angulos(:,9);
angulosCorrecao(:,4) = angulos(:,10);
angulosCorrecao(:,5) = angulos(:,5);
angulosCorrecao(:,6) = angulos(:,6);
 angulosCorrecao(:,7) = angulos(:,11);
 angulosCorrecao(:,8) = angulos(:,12);
angulosCorrecao(:,9) = angulos(:,3);
angulosCorrecao(:,10) = angulos(:,4);
angulosCorrecao(:,11) = angulos(:,8);
angulosCorrecao(:,12) = angulos(:,9);
 
% for coluna = 7:12
% 	angulosCorrecao(:,coluna) = - angulosCorrecao(:,coluna);
% end
 
 
 printf('\n{');
 for linha = 1:41 
 	printf('{');
 	printf(' %i ',angulosCorrecao(linha,1));
 	for coluna = 2:12 
 		printf(', %i ',angulosCorrecao(linha,coluna)); 
 	end 

 	printf('}' );
 	 if linha == 41 
 		break
 	end  
 	 printf(', \n' );
 end
 printf('}'); 
 		
 	
  
  