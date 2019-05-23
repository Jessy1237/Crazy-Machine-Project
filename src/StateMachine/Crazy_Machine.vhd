-- Copyright (C) 1991-2012 Altera Corporation
-- Your use of Altera Corporation's design tools, logic functions 
-- and other software and tools, and its AMPP partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Altera Program License 
-- Subscription Agreement, Altera MegaCore Function License 
-- Agreement, or other applicable license agreement, including, 
-- without limitation, that your use is for the sole purpose of 
-- programming logic devices manufactured by Altera and sold by 
-- Altera or its authorized distributors.  Please refer to the 
-- applicable agreement for further details.

-- Generated by Quartus II Version 12.1 Build 177 11/07/2012 SJ Web Edition
-- Created on Sat May 18 16:58:07 2019

LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY Crazy_Machine IS
    PORT (
        reset : IN STD_LOGIC := '0';
        clock : IN STD_LOGIC;
        Ppad2_I : IN STD_LOGIC := '0';
        Ppad3_I : IN STD_LOGIC := '0';
        sw9 : IN STD_LOGIC := '0';
        sw8 : IN STD_LOGIC := '0';
        led9 : OUT STD_LOGIC;
        led8 : OUT STD_LOGIC;
        led7 : OUT STD_LOGIC;
        led6 : OUT STD_LOGIC;
        led5 : OUT STD_LOGIC;
        led4 : OUT STD_LOGIC;
        StateBit_1 : OUT STD_LOGIC;
        StateBit_2 : OUT STD_LOGIC;
        StateBit_3 : OUT STD_LOGIC;
        servo1: OUT STD_LOGIC;
		  servo2: OUT STD_LOGIC
    );
END Crazy_Machine;

ARCHITECTURE BEHAVIOR OF Crazy_Machine IS
	 
	 COMPONENT Timer
		PORT(
			clk     : in std_logic;
			reset    : in std_logic; -- Negative reset
			Millis : inout integer;
			Seconds : inout integer;
			Minutes : inout integer
		);
end COMPONENT;
    
    COMPONENT servo_pwm1
        PORT (
            clk   : IN  STD_LOGIC;
            reset : IN  STD_LOGIC;
            pos1   : IN INTEGER range 0 to 180;
            servo1 : OUT STD_LOGIC
        );
    END COMPONENT;
	 
	 COMPONENT servo_pwm2
        PORT (
            clk   : IN  STD_LOGIC;
            reset : IN  STD_LOGIC;
            pos2   : IN INTEGER range 0 to 180;
            servo2 : OUT STD_LOGIC
        );
    END COMPONENT;
	 
	 signal clk_out : STD_LOGIC := '0';
    TYPE type_fstate IS (State000,State001,State010,State011,State100,State101);
    SIGNAL fstate : type_fstate;
    SIGNAL reg_fstate : type_fstate;
	 SIGNAL pos1 : INTEGER range 0 to 180;
	 SIGNAL pos2 : INTEGER range 0 to 180;
	 SIGNAL Millis : INTEGER;
	 SIGNAL Seconds : INTEGER;
	 SIGNAL Minutes : INTEGER;
	 SIGNAL started : STD_LOGIC := '0';
	 
BEGIN
	 
	 Timer_map: Timer PORT MAP(
        clock, started, Millis, Seconds, Minutes
    );
    
    servo_pwm1_map: servo_pwm1 PORT MAP(
        clock, reset, pos1, servo1
    );
	 
    servo_pwm2_map: servo_pwm2 PORT MAP(
        clock, reset, pos2, servo2
    );

    PROCESS (clock,reg_fstate)
    BEGIN
        IF (clock='1' AND clock'event) THEN
            fstate <= reg_fstate;
        END IF;
    END PROCESS;

    PROCESS (fstate,reset,Ppad2_I,Ppad3_I,sw9,sw8)
    BEGIN
        IF (reset='1') THEN
            reg_fstate <= State000;
            led9 <= '0';
            led8 <= '0';
            led7 <= '0';
            led6 <= '0';
            led5 <= '0';
            led4 <= '0';
            StateBit_1 <= '0';
            StateBit_2 <= '0';
            StateBit_3 <= '0';
			   pos2 <= 0;
	         pos1 <= 0;
				started <= '0';
        ELSE
            led9 <= '0';
            led8 <= '0';
            led7 <= '0';
            led6 <= '0';
            led5 <= '0';
            led4 <= '0';
            StateBit_1 <= '0';
            StateBit_2 <= '0';
            StateBit_3 <= '0';
			   pos2 <= 0;
	         pos1 <= 0;
            CASE fstate IS
                WHEN State000 =>
                    IF ((sw9 = '1')) THEN
                        reg_fstate <= State001;
                    -- Inserting 'else' block to prevent latch inference
                    ELSE
                        reg_fstate <= State000;
                    END IF;

						  started <= '0';
						  
                    StateBit_3 <= '0';

                    led9 <= '1';

                    StateBit_2 <= '0';

                    StateBit_1 <= '0';
						  
						  IF(( Seconds = 10)) THEN
								pos2 <= 90;
						  END IF;
						  
                WHEN State001 =>
					 IF(started = '0') THEN
					 started <= '1';
					 END IF;
                    IF ((sw8 = '1')) THEN
                        reg_fstate <= State010;
                    -- Inserting 'else' block to prevent latch inference
                    ELSE
                        reg_fstate <= State001;
                    END IF;

                    StateBit_3 <= '1';

                    led8 <= '1';

                    StateBit_2 <= '0';

                    StateBit_1 <= '0';
						 
						 
						  IF((Seconds = 10)) THEN
			           pos1 <= 90;
						  END IF;

                WHEN State010 =>
                    IF ((Ppad2_I = '1')) THEN
                        reg_fstate <= State011;
                    -- Inserting 'else' block to prevent latch inference
                    ELSE
                        reg_fstate <= State010;
                    END IF;

                    StateBit_3 <= '0';

                    StateBit_2 <= '1';

                    StateBit_1 <= '0';

                    led7 <= '1';
                WHEN State011 =>
                    IF ((Ppad3_I = '1')) THEN
                        reg_fstate <= State100;
                    -- Inserting 'else' block to prevent latch inference
                    ELSE
                        reg_fstate <= State011;
                    END IF;

                    StateBit_3 <= '1';

                    StateBit_2 <= '1';

                    led6 <= '1';

                    StateBit_1 <= '0';
                WHEN State100 =>
                    IF ((Ppad2_I = '1')) THEN
                        reg_fstate <= State101;
                    -- Inserting 'else' block to prevent latch inference
                    ELSE
                        reg_fstate <= State100;
                    END IF;

                    led5 <= '1';

                    StateBit_3 <= '0';

                    StateBit_2 <= '0';

                    StateBit_1 <= '1';
                WHEN State101 =>
                    IF ((Ppad3_I = '1')) THEN
                        reg_fstate <= State000;
                    -- Inserting 'else' block to prevent latch inference
                    ELSE
                        reg_fstate <= State101;
                    END IF;

                    led4 <= '1';

                    StateBit_3 <= '1';

                    StateBit_2 <= '0';

                    StateBit_1 <= '1';
                WHEN OTHERS => 
                    led9 <= 'X';
                    led8 <= 'X';
                    led7 <= 'X';
                    led6 <= 'X';
                    led5 <= 'X';
                    led4 <= 'X';
                    StateBit_1 <= 'X';
                    StateBit_2 <= 'X';
                    StateBit_3 <= 'X';
                    report "Reach undefined state";
            END CASE;
        END IF;
    END PROCESS;
END BEHAVIOR;
