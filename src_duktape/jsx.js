reload();
console.clear();

function JSX(name, props, children) {
  console.log('name', name);
  console.log('props', props);
  console.log('children', children);
}

function Wat() {
  console.log("Called Wat");
  return <div>Hello to Wat</div>;
}

x = <Wat>
  <Wat></Wat>
</Wat>

