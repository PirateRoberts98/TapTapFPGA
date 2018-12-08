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
entity game_clock is 
    port(
    clk: in std_logic ;
    clk_game : out std_logic  );
end game_clock ; 


------------------------------------------------
--
--
--
--
--
------------------------------------------------
architecture linearGameClock of game_clock is
    constant SECOND : integer := 49999999;
    signal pulse : integer range 0 to 50000000 ; 

begin
    if (pulse == 0) then
        clk_game <= '1' ;
        pulse <= SECOND ; 
    else 
        clk_game <= '0' ;
        pulse < pulse - 1 ; 
    end if ; 

end linearGameClock ; -- linearGameClock