function show_states( states ) 
	% mostra a trajetoria em cada motor
	figure; 
	for pos = 1:12
		subplot(6,2,pos);
		plot(states(:,pos) );
	end
end
	
	
	