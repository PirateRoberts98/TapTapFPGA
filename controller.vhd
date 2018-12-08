library ieee;
use IEEE.Std_Logic_1164.all;

entity gameCtr is
port(
	 --key_2 : in std_logic ;
	 --key_1 : in std_logic ;
	 --key_0 : in std_logic ;
     keyInputs : in std_logic_vector(2 downto 0); -- Vector version of Keys 
	 CLK : in std_logic;
	 EN : in std_logic;
	 RST : in std_logic;
	 output : out std_logic_vector(2 downto 0)
);
end gameCtr;

architecture gameCtr_arc of gameCtr is
begin 
	output<="000";--set default value;
	process(CLK, keyInputs)
	begin
		if RST = '0' then
			output <= "000";
		elsif (CLK'event and CLK = '1') then
			if EN = '1' then	
				output <= keyInputs;
			end if;
		end if;
	end process;
end gameCtr_arc;