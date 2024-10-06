import React from 'react';
import { useNavigate } from 'react-router-dom'; // Assuming you are using react-router-dom for navigation

const TestTables = ({ data }) => {
  const navigate = useNavigate();

  const handleRowClick = (id) => {
    navigate(`/details/${id}`); // Navigate to the new page with the clicked row ID
  };

  return (
    <div className="overflow-x-auto">
      <table className="min-w-full table-auto border-collapse border border-gray-200">
        <thead className="bg-gray-200">
          <tr>
            <th className="px-4 py-2 border">ID</th>
            <th className="px-4 py-2 border">Name</th>
            <th className="px-4 py-2 border">Test Type</th>
            <th className="px-4 py-2 border">Buttons</th>
          </tr>
        </thead>
        <tbody>
          {data.map((row, index) => (
            <tr
              key={row.id}
              onClick={() => handleRowClick(row.id)}
              className={`cursor-pointer ${index % 2 === 0 ? 'bg-white' : 'bg-gray-100'} hover:bg-gray-300`}
            >
              <td className="px-4 py-2 border">{row.id}</td>
              <td className="px-4 py-2 border">{row.name}</td>
              <td className="px-4 py-2 border">{row.testType}</td>
              <td className="px-4 py-2 border">
                <button className="px-2 py-1 bg-blue-500 text-white rounded">Action</button>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default TestTables;
