liabrary ieee;
use IEEE.Std_logic_1164.all;

entity gameMem is
port(
	rngIn : in Std_logic_vector(2 downto 0); --- input from pseudo RNG generate;
	keyIn : in Std_logic_vector(2 downto 0); ---input from the keyboard;
	CLK : in std_logic;
	EN : in std_logic;
	start :in std_logic; --start key for the game
	RST : in std_logic;
	gameOver : out std_logic;
	visualOut : out std_logic_vector(2 downto 0); --- output for VGA
	point : out unsigned(7 downto 0) --- output for the points(use 7 segma display)
);

architecture gameMen_ARC of gameMem is
	type gameState: idle,load,delay,comapre,over;
	signal Clk,en, RST,start;
	signal rngIn,rngIn_current,keyIn,keyIn_current;--keyIn is the input from the key input by user and rngIn is the input from the rng generator.  
	signal visualOut;
	signal point,point_next;
	signal state_reg,state_next:gameState;
	
	--state logic
	process(EN,Clk)
	begin
		if EN ='0' then 
			state_reg <= idle;
		elsif clk'event and clk = '1' then
			state_reg <= state_next;
		end if;
	end process;
	--initialization 
	point<="00000000";
	visualOut<="000";
	gameOver <='0';
	--next state and data logic
	process(all)
	begin
		case state_next
			when idle =>
				if start ='1' then
					state_next <= load;
				else
					state_next <= idle;
				end if
			
			when load =>
				rngIn_current<=rngIn; --load the next rng number to the register
				keyIn_current<=keyIn;-- load the user input;
				visualOut<=rngIn;
				if keyIn = "000" then
					state_next <= delay; ---waiting for user input
				else 
					state_next <= compare;
				end if
				
			when delay =>
				state_next<=compare; -- delay the system for one clock cycle for now and waiting for user input
			
			when comapre =>
				if rngIn_current xor keyIn_current ='1' then -- compare the user input with rng output
					point_next <= point_next + 1;
					point<= point_next;
					keyIn_current<="000";
					state_next <= load;
				
				else 
					state_next <= over;
				end if;
			
			when over =>
				keyIn_current,rngIn_current <= "000";
				point<="00000000";
				visualOut<="000";
				gameOver <='1';
			    state_next <= idle;
		end case;
	end process;
end gameMen_ARC;