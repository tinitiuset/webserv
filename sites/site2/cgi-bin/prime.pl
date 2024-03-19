#!/usr/bin/perl



eval {
    my $query_string = $ENV{'QUERY_STRING'};

    if (defined $query_string and $query_string ne "") {
        my ($key, $num) = split '=', $query_string;

        if ($num =~ /^\d+$/) {
            if (is_prime($num)) {
                print "$num is prime.";
            } else {
                print "$num is not prime.";
            }
        } else {
            print "Error: $num is not an integer.";
        }
    } else {
        print "Error: QUERY_STRING is not present or empty.";
    }
};

if ($@) {
    print "Error: An unexpected error occurred.";
}

sub is_prime {
    my $num = shift;
    return 0 if $num < 2;
    for (my $i = 2; $i * $i <= $num; $i++) {
        return 0 if $num % $i == 0;
    }
    return 1;
}
