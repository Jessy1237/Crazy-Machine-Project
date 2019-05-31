library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity servo_pwm4 is
    PORT (
        clk   : IN  STD_LOGIC;
        reset : IN  STD_LOGIC;
        pos4   : IN  INTEGER range 0 to 360;
        servo4 : OUT STD_LOGIC
    );
end servo_pwm4;

ARCHITECTURE rtl OF servo_pwm4 IS
 
    -- Signal for counting clock periods
    SIGNAL Ticks : INTEGER;
	 SIGNAL degrees : INTEGER;
 
begin
    process(clk) is
    begin
        if rising_edge(clk) then
 
            -- If the negative reset signal is active
            if reset = '1' then
                Ticks   <= 0;
					 degrees <= 0;
            else
 
                -- True every 180th of 1 ms. i.e divides our clock so we can count every degree per millisecond.
                if Ticks = 277 then
                    Ticks <= 0;
						  
							-- there are 3600 degrees in 20ms, so reset our degrees to 0 so that we can start the next pulse.
							if degrees = 3599 then
								degrees <=0;
							else
								degrees <= degrees +1;
							end if;
						  
                else
                    Ticks <= Ticks + 1;
                end if;
 
            end if;
        end if;
    end process;
	  -- Output signal for the servomotor. Outputs between a range of Pulse Width of 1ms to 2ms for 0 to 180 degrees servo change. With a period of PWM being 20ms i.e. 50Hz
    servo4 <= '1' when (degrees < (180 + pos4)) else '0';
end architecture;