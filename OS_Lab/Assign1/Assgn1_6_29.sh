> $2
N=1000000
for ((i=2; i<=N; i++)); do
  primes[i]=1
done

for ((i=2; i<=N; i++)); do
  if [ ${primes[i]} -eq 1 ]; then
    for ((j=i*i; j<=N; j+=i)); do
      primes[j]=0
    done
  fi
done

while read n;
do
    n=$((n))
    for ((i=2; i<=n; i++)); do
    if [ ${primes[i]} -eq 1 ]; then
        echo -n $i "" >> $2
    fi
    done
    echo "" >> $2
done <$1