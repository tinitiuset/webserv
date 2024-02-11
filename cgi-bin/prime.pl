#!/usr/bin/perl


my $query_string = $ENV{'QUERY_STRING'};
my ($key, $num) = split '=', $query_string;


if (is_prime($num)) {
    print "$num is prime.";
} else {
    print "$num is not prime.";
}

# Función para verificar si un número es primo
sub is_prime {
    my $num = shift;
    return 0 if $num < 2;
    for (my $i = 2; $i * $i <= $num; $i++) {
        return 0 if $num % $i == 0;
    }
    return 1;
}
