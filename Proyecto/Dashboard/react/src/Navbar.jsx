import styled from 'styled-components'
function Navbar() {
  return (
    <>
       <NavContainer >
       <h2 >SO2<span>P1</span></h2>
       <div> 
       <a  href="/dashboard"> Dashboard</a>
        <a  href="/otros"> Otros</a>

       </div>
     </NavContainer>
    </>
  )
}

export default Navbar

const NavContainer = styled.nav `
h2 {
    color: white;
    font-weight: 900;
    span{
        font-weight: bold;
    }  

}  
padding: .4rem;
background-color:#8f2424;
display: flex;
align-items: center;
justify-content: space-between;
a{
color: white;
text-decoration: none;
margin-right:1rem;
font-size: 2rem;


} 
`