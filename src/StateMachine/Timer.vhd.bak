library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
 
entity Timer is
port(
    clk     : in std_logic;
    reset    : in std_logic; -- Negative reset
	 Millis : inout integer;
    Seconds : inout integer;
    Minutes : inout integer);
end entity;
 
architecture rtl of Timer is
 
    -- Signal for counting clock periods
    signal Ticks : integer;
 
begin
 
    process(clk) is
    begin
        if rising_edge(clk) then
 
            -- If the negative reset signal is active
            if reset = '0' then
                Ticks   <= 0;
					 Millis <= 0;
                Seconds <= 0;
                Minutes <= 0;
            else
 
                -- True once every millisecond
                if Ticks = 49999 then
                    Ticks <= 0;
							
							if Millis = 999 then
							Millis <= 0;
 
								-- True once every minute
								if Seconds = 59 then
									Seconds <= 0;
 
									-- True once every hour
									if Minutes = 59 then
										Minutes <= 0;
									else
										Minutes <= Minutes + 1;
									end if;
 
								else
									Seconds <= Seconds + 1;
								end if;
								
							else
								Millis <= Millis + 1;
							end if;
 
                else
                    Ticks <= Ticks + 1;
                end if;
 
            end if;
        end if;
    end process;
end architecture;