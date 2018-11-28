library ieee ; 
use ieee.std_1164.all ; 
------------------------------------------------
--
--
--
--
--
------------------------------------------------



------------------------------------------------
--
--
--
--
--
------------------------------------------------
entity RNG_reg is 
port(
clk : in std_logic ;
en : in std_logic ; 
mod_0 : out std_logic ; 
mod_1 : out std_logic ; 
mod_2 : out std_logic );
end RNG_reg ; 


------------------------------------------------
--
--
--
--
------------------------------------------------
architecture random_register of RNG_reg is
    constant SEED : integer := '01101001' ; 
    signal value : std_logic_vector(7 downto o) := SEED ; 

begin

end random_register ; -- random_register