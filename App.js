import React, { useEffect, useState, useRef } from 'react';
import './App.css';

function App() {
  // State to display status messages to the user
  const [statusMessage, setStatusMessage] = useState('');

  // State to log actions like Lock/Unlock with timestamps
  const [log, setLog] = useState([]);

  // Ref to track the locked status without causing unnecessary renders
  const isLocked = useRef(false);

  // State to manage authentication status
  const [isAuthenticated, setIsAuthenticated] = useState(false);

  // States for user login credentials
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');

  // Credentials for login validation
  const testUsername = 'admin';
  const testPassword = 'password123';

  // Polling function to check for manual lock/unlock status every 3 seconds
  useEffect(() => {
    const interval = setInterval(checkManualLockStatus, 3000);
    return () => clearInterval(interval); // Cleanup interval on component unmount
  }, []);

  /**
   * Fetch manual lock status from the backend.
   * Updates the UI and log based on the manual status.
   */
  const checkManualLockStatus = async () => {
    try {
      const response = await fetch('https://d1f0-47-151-162-101.ngrok-free.app/data/status');
      if (response.ok) {
        const status = await response.json();
        
        // Update status if the device is manually locked
        if (status.manualStatus === '3' && !isLocked.current) {
          setStatusMessage('Device was manually locked');
          logAction('Manual Lock');
          isLocked.current = true;
        }
        // Update status if the device is manually unlocked
        else if (status.manualStatus === '4' && isLocked.current) {
          setStatusMessage('Device was manually unlocked');
          logAction('Manual Unlock');
          isLocked.current = false;
        }
      } else {
        console.error('Failed to fetch manual lock status');
      }
    } catch (error) {
      console.error('Error checking manual lock status:', error);
    }
  };

  // Check if user is already authenticated on page load
  useEffect(() => {
    const storedAuth = localStorage.getItem('isAuthenticated');
    if (storedAuth === 'true') {
      setIsAuthenticated(true);
    }
  }, []);

  /**
   * Handle user login validation
   */
  const handleLogin = (e) => {
    e.preventDefault();
    if (username === testUsername && password === testPassword) {
      setIsAuthenticated(true);
      localStorage.setItem('isAuthenticated', 'true'); // Store login status in localStorage
    } else {
      alert('Invalid credentials');
    }
  };

  /**
   * Logout function: resets authentication status
   */
  const handleLogout = () => {
    setIsAuthenticated(false);
    localStorage.removeItem('isAuthenticated'); // Clear authentication flag
  };

  /**
   * Log actions like Lock/Unlock with a timestamp
   */
  const logAction = (action) => {
    const timestamp = new Date().toLocaleString();
    setLog((prevLog) => [{ action, timestamp }, ...prevLog]);
  };

  /**
   * Handle locking the device.
   * Sends a request to the backend to perform the lock action.
   */
  const handleLockClick = async (event) => {
    event.preventDefault();
    setStatusMessage('Locking the device...');
    logAction('Lock'); // Log the Lock action
    try {
      const response = await fetch('https://d1f0-47-151-162-101.ngrok-free.app/data/2', {
        method: 'GET',
        headers: {
          'ngrok-skip-browser-warning': '69420',
        },
      });
      if (!response.ok) throw new Error('Network response was not ok');
    } catch (error) {
      console.error('There was a problem with the fetch operation:', error);
    }
    isLocked.current = true; // Update lock status
    setTimeout(() => {
      setStatusMessage('Device is locked');
    }, 1000);
  };
  
  /**
   * Handle unlocking the device.
   * Sends a request to the backend to perform the unlock action.
   */
  const handleUnlockClick = async (event) => {
    event.preventDefault();
    setStatusMessage('Unlocking the device...');
    logAction('Unlock'); // Log the Unlock action
    try {
      const response = await fetch('https://d1f0-47-151-162-101.ngrok-free.app/data/1', {
        method: 'GET',
        headers: {
          'ngrok-skip-browser-warning': '69420',
        },
      });
      if (!response.ok) throw new Error('Network response was not ok');
    } catch (error) {
      console.error('There was a problem with the fetch operation:', error);
    }
    isLocked.current = false; // Update lock status
    setTimeout(() => {
      setStatusMessage('Device is unlocked');
    }, 1000);
  };

  /**
   * YouTube video player component to embed a video
   */
  function YouTubePlayer({ videoId }) {
    return (
      <div className="video-container">
        <iframe
          src={`https://www.youtube.com/embed/${videoId}?autoplay=1`}
          allowFullScreen
          frameBorder="0"
          allow="autoplay; encrypted-media"
        ></iframe>
      </div>
    );
  }

  return (
    <div className="App">
      {/* Conditional Rendering for Authentication */}
      {!isAuthenticated ? (
        <div>
          <h2>Login</h2>
          <form onSubmit={handleLogin}>
            <div>
              <label>Username:</label>
              <input
                type="text"
                value={username}
                onChange={(e) => setUsername(e.target.value)}
              />
            </div>
            <div>
              <label>Password:</label>
              <input
                type="password"
                value={password}
                onChange={(e) => setPassword(e.target.value)}
              />
            </div>
            <button type="submit">Login</button>
          </form>
        </div>
      ) : (
        <div>
          {/* Main Smart Lock Interface */}
          <h1>Smart Lock Box</h1>
          <button onClick={handleLogout} className="logout-button">Logout</button>
          
          {/* Embed YouTube Video */}
          <YouTubePlayer videoId="g7yHYenbHWQ" />
          
          {/* Lock/Unlock Buttons */}
          <div>
            <button type="button" onClick={handleLockClick}>Lock</button>
            <button type="button" onClick={handleUnlockClick}>Unlock</button>
          </div>
          
          {/* Status Message */}
          <p>{statusMessage}</p>
          
          {/* Log Table */}
          <div className="log-table-container">
            <div className="log-table">
              <table>
                <thead>
                  <tr>
                    <th>#</th>
                    <th>Action</th>
                    <th>Timestamp</th>
                  </tr>
                </thead>
                <tbody>
                  {log.map((entry, index) => (
                    <tr key={index}>
                      <td>{index + 1}</td>
                      <td>{entry.action}</td>
                      <td>{entry.timestamp}</td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </div>
        </div>
      )}
    </div>
  );
}

export default App;
