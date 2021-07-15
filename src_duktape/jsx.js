console.clear();

function JSX(a, b, c) {
  console.log('a', a);
  console.log('b', b);
  console.log('c', c);
}

function Wat() {
  console.log("Called Wat");
  return <div>Hello to War</div>;
}

x = <Wat>
  <Wat></Wat>
</Wat>
