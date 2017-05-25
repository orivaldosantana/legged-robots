function imprime_c_matriz(matriz) 
	num_linhas = length(matriz(:,1));
	 printf('\n{');
	 for linha = 1:num_linhas 
	 	printf('{');
	 	printf(' %i ',matriz(linha,1));
	 	for coluna = 2:12 
	 		printf(', %i ',matriz(linha,coluna)); 
	 	end 
	
	 	printf('}' );
	 	 if linha == num_linhas
	 		break
	 	end  
	 	 printf(', \n' );
	 	 if (linha > 1000)
	 	 	break
	 	 end
	 end
	% printf('};'); 
	
end