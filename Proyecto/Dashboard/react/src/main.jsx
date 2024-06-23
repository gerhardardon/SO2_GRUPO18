/*import React from 'react'
import ReactDOM from 'react-dom/client'
import App from './App.jsx'

ReactDOM.createRoot(document.getElementById('root')).render(
  <React.StrictMode>
    <App />
  </React.StrictMode>,
)
*/

import React from 'react'
import * as ReactDOM from 'react-dom/client'
import App from './App.jsx'
import { createBrowserRouter, RouterProvider, Route} from 'react-router-dom'
import Navbar from './Navbar'
import Dashboard from './Dashboard.jsx'
import Otros from './otros.jsx'


function Home(){
  return (
    <>
    <Navbar/>
  </>
  )
}

function Dashboardd(){
  return (
    <div>
      <Navbar/>
     <Dashboard/>
    
  </div>
  )
}
function Otross(){
  return (
    <div>
      <Navbar/>
     <Otros/>
    
  </div>
  )
}
const router = createBrowserRouter([ 
  {
    path: "/",
    element: <Home/>,
  },
  {
    path: "/dashboard",
    element: <Dashboardd/>,
  },
  {
    path: "/otros",
    element: <Otross/>,
  },
] )


ReactDOM.createRoot(document.getElementById('root')).render(
  <React.StrictMode>
    <RouterProvider router={router}/>
  </React.StrictMode>,

)
export default router;