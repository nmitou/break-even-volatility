# break-even-volatility
Repo for calculating the break-even volatility of a security, or the skew/surface thereof for multiple strikes/terms.

Building:
install Eigen

when compiling, include eigen path
compile each file first into object files, including eigen where necessary
link object files after

Notes:

time to maturity:
will treat data as daily spot prices. from the formula (adding daily hedging errors to get pnl) so data will be on
different days and then time to maturity in formula will be remaining days/252. assuming data given is only available in business days
ie the user will have filtered days which should not have any data
simplifying assumption: 21 days per mpnth
maturities vector entered with months
