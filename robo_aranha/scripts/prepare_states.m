function prepare_states ( basic_patterns,mult_patterns) 
	
	% ajusta dimensao 
	[l c] = size(basic_patterns);
	scaled_patterns = zeros(l,c);
	 for coluna = 1:c
		if ( mod(coluna,2) == 0 )
			scaled_patterns(:,coluna) = -30 + basic_patterns(:,coluna)*50;
		else
			scaled_patterns(:,coluna) = - basic_patterns(:,coluna)*45;
		end
		if ( coluna > 6) 
			scaled_patterns(:,coluna) = - scaled_patterns(:,coluna) ;
		end
	end
	
	
	% coloca mais padroes, por interpolacao
	%mult_patterns = 4;
	more_patterns = zeros((l-1)*mult_patterns,c);
	for pos = 1:(l-1)
		delta = (scaled_patterns(pos+1,:) - scaled_patterns(pos,:))/mult_patterns;
	
		mult_line =  (pos -1) * (mult_patterns)+1;
		more_patterns(mult_line,:) = scaled_patterns(pos,:); 
		for cont_mult = 1:mult_patterns 	
			more_patterns(mult_line + cont_mult,:) = scaled_patterns(pos,:) + cont_mult * delta ;
		end 
	end
	% mostra a trajetoria em cada motor
	show_states(more_patterns);

	[l c] = size(more_patterns);
	printf("states [%i][%i] = ",l,c);
	%imprime_c_matriz(scaled_patterns);
	imprime_c_matriz(more_patterns);
	
	
end