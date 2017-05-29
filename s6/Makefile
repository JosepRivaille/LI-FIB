coins = coins
squares = squares
tomografia = tomografia

$(coins): $(coins).pl
	swipl -quiet -O -g main --stand_alone=true -o $(coins) -c $(coins).pl

$(squares): $(squares).pl
	swipl -quiet -O -g main --stand_alone=true -o $(squares) -c $(squares).pl

$(tomografia): $(tomografia).pl
	swipl -quiet -O -g main --stand_alone=true -o $(tomografia) -c $(tomografia).pl
