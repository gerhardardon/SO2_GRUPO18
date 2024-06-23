import React, { useEffect, useState } from 'react';
import axios from 'axios';
import './otros.css';

const Otros = () => {
    const [proc, setProc] = useState([]);

    // Función para obtener datos de la API
    const fetchProc = () => {
        axios.get('http://localhost:5200/api/data/todos_procesos')
        .then(response => {
          setProc(response.data);
        })
        .catch(error => {
          console.error('Error fetching data:', error);
        });
}
useEffect(() => {
  
    fetchProc();
    // Llamar a fetchData cada 5 segundos
    const intervalId = setInterval(() => {
      fetchProc();
    }, 5000);

    // Limpiar el intervalo cuando el componente se desmonte
    return () => clearInterval(intervalId);
  }, []);

  return (
    <div>
        
      <div className="dashboard-title2-container">
        <h1 className="dashboard-title2">Todos los procesos</h1>
      </div>
      <div className="dashboard-table3-container">
        <table className="dashboard-table3">
          <thead>
            <tr>
              <th colSpan="4">Todos los Procesos</th> {/* Título para la segunda tabla */}
            </tr>
            <tr>
              <th>pid</th>
              <th>Nombre</th>
              <th>Memoria</th>
              <th>Porcentaje</th>
            </tr>
          </thead>
          <tbody>
            {proc.map(proceso => (
              <tr key={proceso.pid}>
                <td>{proceso.pid}</td>
                <td>{proceso.nombre}</td>
                <td>{proceso.memoriamb}</td>
                <td>{proceso.porcentaje_memoria * 100}%</td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
  
    </div>
  );
};

export default Otros;